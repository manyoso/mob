#include "messageserver.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTime>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#define DEBUG_MESSAGESERVER 0

class MessageThread : public QThread {
    Q_OBJECT
public:
    virtual ~MessageThread();

signals:
    void socketError(QAbstractSocket::SocketError socketError);
    void receivedMessage(QSharedPointer<Message>);

protected:
    virtual void run();

private:
    friend class MessageServerPrivate;
    MessageThread(int socketDescriptor);
    void readSocket(QTcpSocket* socket);

    int m_socketDescriptor;
    bool m_firstRead;
    quint16 m_typeOfMessage;
    quint32 m_sizeOfMessage;
    QHostAddress m_address;
    quint16 m_port;
};

MessageThread::MessageThread(int socketDescriptor)
    : QThread(0)
    , m_socketDescriptor(socketDescriptor)
    , m_firstRead(true)
    , m_typeOfMessage(0)
    , m_sizeOfMessage(0)
    , m_port(0)
{
}

MessageThread::~MessageThread()
{
    if (!wait(1000)) {
        terminate();
        wait();
    }
}

void MessageThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(m_socketDescriptor)) {
        emit socketError(tcpSocket.error());
        return;
    }

    Q_ASSERT(tcpSocket.state() == QAbstractSocket::ConnectedState);
    if (tcpSocket.state() != QAbstractSocket::ConnectedState) {
        emit socketError(QAbstractSocket::UnknownSocketError);
        return;
    }

    m_address = tcpSocket.peerAddress();
    m_port = tcpSocket.peerPort();

#if DEBUG_MESSAGESERVER
    qDebug() << "Starting new connection thread with" << m_address << "on" << m_port;
#endif

    while (tcpSocket.state() == QAbstractSocket::ConnectedState) {
        // Need at least 5 bytes to begin reading the message
        if (tcpSocket.bytesAvailable() >= (m_firstRead ? 5 : m_sizeOfMessage)) {
            readSocket(&tcpSocket);
            m_firstRead = false;
        }
#if DEBUG_MESSAGESERVER
        qDebug() << "Waiting for socket read in state" << tcpSocket.state();
#endif
        tcpSocket.waitForReadyRead();
    }
}

void MessageThread::readSocket(QTcpSocket* socket)
{
    if (m_firstRead) {
        socket->read(reinterpret_cast<char*>(&m_typeOfMessage), sizeof(quint16));
#if DEBUG_MESSAGESERVER
        qDebug() << "Reading type" << m_typeOfMessage << "from socket with" << socket->bytesAvailable() << "bytesAvailable";
#endif

        socket->read(reinterpret_cast<char*>(&m_sizeOfMessage), sizeof(quint32));
#if DEBUG_MESSAGESERVER
        qDebug() << "Reading size" << m_sizeOfMessage << "from socket with" << socket->bytesAvailable() << "bytesAvailable";
#endif
    }

    if (socket->bytesAvailable() < m_sizeOfMessage) {
#if DEBUG_MESSAGESERVER
        qDebug() << "Waiting for more bytes because" << socket->bytesAvailable() << "<" << m_sizeOfMessage;
#endif
        return;
    }

#if DEBUG_MESSAGESERVER
    qDebug() << "Reading message from socket of size" << socket->bytesAvailable();
#endif

    QSharedPointer<Message> msg(Message::createMessage(Message::Type(m_typeOfMessage)));
    if (!msg) {
        qDebug() << "ERROR: Unrecognized message type" << m_typeOfMessage << "!";
        QCoreApplication::exit(1);
    }

    QDataStream stream(socket);
    stream >> *msg;

    if (!msg->deserialize(socket))
        qDebug() << "ERROR: Receiving message could not deserialize the message directly from the socket!";

    msg->setOrigin(m_address);
    emit receivedMessage(msg);
}

class MessageServerPrivate : public QTcpServer {
    Q_OBJECT
public:
    MessageServerPrivate(QObject* parent, const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort);
    virtual ~MessageServerPrivate();

    // Reimplemented from QTcpServer
    virtual void incomingConnection(int socketDescriptor);

    QNetworkAddressEntry m_networkAddress;
    quint16 m_readPort;
    quint16 m_writePort;
    QTcpSocket *m_tcpServer;
    QTcpSocket *m_tcpSocket;
    QHash< MessageFilter, QWeakPointer<MessageHandler> > m_handlers;
    QSet< QSharedPointer<MessageThread> > m_threads;

public slots:
    void messageThreadFinished();
    void socketError(QAbstractSocket::SocketError);
    void connectedSocketError(QAbstractSocket::SocketError);
    void receivedMessageInternal(QSharedPointer<Message>);
    bool receivedMessageInternal(QSharedPointer<Message>, const MessageFilter&);
};

MessageServerPrivate::MessageServerPrivate(QObject* parent, const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort)
    : QTcpServer(parent)
    , m_networkAddress(address)
    , m_readPort(readPort)
    , m_writePort(writePort)
    , m_tcpServer(0)
    , m_tcpSocket(0)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType< QSharedPointer<Message> >("QSharedPointer<Message>");

    // Listens for incoming messages
    if (!listen(QHostAddress::Any, m_readPort)) {
        qDebug() << "ERROR: Could not start the TCP server!";
        exit(1);
    }

    // Writes outgoing messages
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
}

MessageServerPrivate::~MessageServerPrivate()
{
    m_handlers.clear();
    m_threads.clear();
}

void MessageServerPrivate::incomingConnection(int socketDescriptor)
{
    QSharedPointer<MessageThread> thread(new MessageThread(socketDescriptor));
    connect(thread.data(), SIGNAL(finished()), this, SLOT(messageThreadFinished()));
    connect(thread.data(), SIGNAL(terminated()), this, SLOT(messageThreadFinished()));
    connect(thread.data(), SIGNAL(socketError(QAbstractSocket::SocketError)),
            this, SLOT(connectedSocketError(QAbstractSocket::SocketError)));
    connect(thread.data(), SIGNAL(receivedMessage(QSharedPointer<Message>)),
            this, SLOT(receivedMessageInternal(QSharedPointer<Message>)), Qt::DirectConnection);
    m_threads.insert(thread);
    thread->start();
}

void MessageServerPrivate::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "ERROR: socket error" << error;
}

void MessageServerPrivate::connectedSocketError(QAbstractSocket::SocketError error)
{
    // We expect the remote host to close this connection so ignore the error
    if (error == QAbstractSocket::RemoteHostClosedError)
        return;

    qDebug() << "ERROR: connected socket error" << error;
}

void MessageServerPrivate::messageThreadFinished()
{
    QWeakPointer<MessageThread> thread(qobject_cast<MessageThread*>(sender()));
    Q_ASSERT(m_threads.contains(thread));
    m_threads.remove(thread);
}

void MessageServerPrivate::receivedMessageInternal(QSharedPointer<Message> msg)
{
    // Full filter
    MessageFilter filter;
    filter.setSessionId(msg->sessionId());
    filter.setType(msg->type());
    filter.setOrigin(msg->origin());

    if (receivedMessageInternal(msg, filter))
        return;

    // From any origin
    filter.setOrigin(QHostAddress::Any);

    if (receivedMessageInternal(msg, filter))
        return;

    // With any type
    filter.setType(Message::Type(-1));
    if (receivedMessageInternal(msg, filter))
        return;

    // With any session id
    filter.setSessionId(QByteArray());
    if (receivedMessageInternal(msg, filter))
        return;

    qDebug() << "ERROR: could not find a handler for message" << msg;
    Q_ASSERT(false);
}

bool MessageServerPrivate::receivedMessageInternal(QSharedPointer<Message> msg, const MessageFilter& filter)
{
    if (!m_handlers.contains(filter))
        return false;

    QWeakPointer<MessageHandler> handler = m_handlers.value(filter);
    QSharedPointer<MessageHandler> strong = handler.toStrongRef();
    if (strong) {
        strong->receivedMessageInternal(msg);
        return true;
    }

    return false;
}

MessageServer::MessageServer(const QNetworkAddressEntry& address, quint16 port, QObject* parent)
    : QObject(parent)
    , d(new MessageServerPrivate(this, address, port, port))
{
}

MessageServer::MessageServer(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent)
    : QObject(parent)
    , d(new MessageServerPrivate(this, address, readPort, writePort))
{
}

MessageServer::~MessageServer()
{
}

quint16 MessageServer::readPort() const
{
    return d->m_readPort;
}

quint16 MessageServer::writePort() const
{
    return d->m_writePort;
}

bool MessageServer::sendMessage(const Message& msg, const QHostAddress& address, bool sync)
{
    if (!d->isListening()) {
        qDebug() << "ERROR: Cannot send a message because the TCP server is not running!";
        return false;
    }

#if DEBUG_MESSAGESERVER
    qDebug() << "Sending message" << msg << "to" << address << "on" << d->m_writePort;
#endif

    d->m_tcpSocket->connectToHost(address, d->m_writePort, QIODevice::WriteOnly);
    if (!d->m_tcpSocket->waitForConnected(1000)) {
        qDebug() << "ERROR: Sending message" << d->m_tcpSocket->error();
        return false;
    }

    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << msg;

    quint16 type = msg.type();
    if (d->m_tcpSocket->write(reinterpret_cast<char*>(&type), sizeof(quint16)) == -1)
        qDebug() << "ERROR: Sending message could not write the type of the message to the socket!";

    quint32 size = bytes.size();
    if (d->m_tcpSocket->write(reinterpret_cast<char*>(&size), sizeof(quint32)) == -1)
        qDebug() << "ERROR: Sending message could not write the size of the message to the socket!";

    if (d->m_tcpSocket->write(bytes) == -1)
        qDebug() << "ERROR: Sending message could not write the message to the socket!";

    if (!msg.serialize(d->m_tcpSocket))
        qDebug() << "ERROR: Sending message could not serialize the message directly to the socket!";

    d->m_tcpSocket->disconnectFromHost();
    if (sync)
        d->m_tcpSocket->waitForDisconnected(-1);
    return true;
}

void MessageServer::installMessageHandler(QSharedPointer<MessageHandler> handler, const MessageFilter& filter)
{
    d->m_handlers.insert(filter, handler);
}

Q_DECLARE_METATYPE(QAbstractSocket::SocketError);
Q_DECLARE_METATYPE(QSharedPointer<Message>);

#include "messageserver.moc"
