#include "messagehandler.h"

#include "node.h"

#include <QtCore/QCoreApplication>

#define DEBUG_MESSAGEHANDLER 0

Q_DECLARE_METATYPE(QAbstractSocket::SocketError);

class ConnectionThread : public QThread {
    Q_OBJECT
public:
    ConnectionThread(int socketDescriptor, MessageHandler* parent);
    virtual ~ConnectionThread();

signals:
    void socketError(QAbstractSocket::SocketError socketError);

protected:
    virtual void run();

private:
    void readSocket(QTcpSocket* socket);
    int m_socketDescriptor;
    bool m_firstRead;
    quint8 m_typeOfMessage;
    quint32 m_sizeOfMessage;
    QHostAddress m_address;
    quint16 m_port;
};

ConnectionThread::ConnectionThread(int socketDescriptor, MessageHandler* parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
    , m_firstRead(true)
    , m_typeOfMessage(0)
    , m_sizeOfMessage(0)
    , m_port(0)
{
}

ConnectionThread::~ConnectionThread()
{
    if (!wait(1000)) {
        terminate();
        wait();
    }
}

void ConnectionThread::run()
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

#if DEBUG_MESSAGEHANDLER
    qDebug() << "Starting new connection thread with" << m_address << "on" << m_port;
#endif

    while (tcpSocket.state() == QAbstractSocket::ConnectedState) {
        // Need at least 5 bytes to begin reading the message
        if (tcpSocket.bytesAvailable() >= (m_firstRead ? 5 : m_sizeOfMessage)) {
            readSocket(&tcpSocket);
            m_firstRead = false;
        }
#if DEBUG_MESSAGEHANDLER
        qDebug() << "Waiting for socket read in state" << tcpSocket.state();
#endif
        tcpSocket.waitForReadyRead();
    }
}

void ConnectionThread::readSocket(QTcpSocket* socket)
{
    if (m_firstRead) {
        socket->read(reinterpret_cast<char*>(&m_typeOfMessage), sizeof(quint8));
#if DEBUG_MESSAGEHANDLER
        qDebug() << "Reading type" << m_typeOfMessage << "from socket with" << socket->bytesAvailable() << "bytesAvailable";
#endif

        socket->read(reinterpret_cast<char*>(&m_sizeOfMessage), sizeof(quint32));
#if DEBUG_MESSAGEHANDLER
        qDebug() << "Reading size" << m_sizeOfMessage << "from socket with" << socket->bytesAvailable() << "bytesAvailable";
#endif
    }

    if (socket->bytesAvailable() < m_sizeOfMessage) {
#if DEBUG_MESSAGEHANDLER
        qDebug() << "Waiting for more bytes because" << socket->bytesAvailable() << "<" << m_sizeOfMessage;
#endif
        return;
    }

#if DEBUG_MESSAGEHANDLER
    qDebug() << "Reading message from socket of size" << socket->bytesAvailable();
#endif

    MessageHandler* handler = qobject_cast<MessageHandler*>(parent());
    Message* msg = Message::createMessage(Message::Type(m_typeOfMessage));
    if (!msg) {
        qDebug() << "ERROR: Unrecognized message type" << m_typeOfMessage << "!";
        QCoreApplication::exit(1);
    }

    QDataStream stream(socket);
    stream >> *msg;

    if (!msg->deserialize(socket))
        qDebug() << "ERROR: Receiving message could not deserialize the message directly from the socket!";

    handler->handleMessageInternal(msg, m_address, m_port);
    delete msg;
}

MessageHandler::MessageHandler(const QNetworkAddressEntry& address, quint16 port, QObject* parent)
    : QTcpServer(parent)
    , m_networkAddress(address)
    , m_readPort(port)
    , m_writePort(port)
    , m_tcpSocket(0)
{
    init();
}

MessageHandler::MessageHandler(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent)
    : QTcpServer(parent)
    , m_networkAddress(address)
    , m_readPort(readPort)
    , m_writePort(writePort)
    , m_tcpSocket(0)
{
    init();
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::init()
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    if (!listen(QHostAddress::Any, readPort())) {
        qDebug() << "ERROR: Could not start the TCP server!";
        exit(1);
    }

    // Writes outgoing messages
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
}

bool MessageHandler::isRunning() const
{
    return isListening();
}

bool MessageHandler::sendMessage(Message* msg, const QHostAddress& address, bool sync)
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot send a message because the TCP server is not running!";
        return false;
    }

#if DEBUG_MESSAGEHANDLER
    qDebug() << "Sending message" << *msg << "to" << address << "on" << m_writePort;
#endif

    m_tcpSocket->connectToHost(address, m_writePort, QIODevice::WriteOnly);
    if (!m_tcpSocket->waitForConnected(1000)) {
        qDebug() << "ERROR: Sending message" << m_tcpSocket->error();
        return false;
    }

    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << *msg;

    quint8 type = msg->type();
    if (m_tcpSocket->write(reinterpret_cast<char*>(&type), sizeof(quint8)) == -1)
        qDebug() << "ERROR: Sending message could not write the type of the message to the socket!";

    quint32 size = bytes.size();
    if (m_tcpSocket->write(reinterpret_cast<char*>(&size), sizeof(quint32)) == -1)
        qDebug() << "ERROR: Sending message could not write the size of the message to the socket!";

    if (m_tcpSocket->write(bytes) == -1)
        qDebug() << "ERROR: Sending message could not write the message to the socket!";

    if (!msg->serialize(m_tcpSocket))
        qDebug() << "ERROR: Sending message could not serialize the message directly to the socket!";

    m_tcpSocket->disconnectFromHost();
    if (sync)
        m_tcpSocket->waitForDisconnected(-1);
    return true;
}

void MessageHandler::expectMessage(const QHostAddress& address)
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot expect message because the TCP server is not running!";
        return;
    }

    {
        QMutexLocker locker(&m_connectWaitMutex);
        m_connectWait = true;
    }

    {
        QMutexLocker locker(&m_messageWaitMutex);
        m_messageWait = address;
    }
}

bool MessageHandler::waitForMessage()
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot wait for message because The TCP server is not running!";
        return false;
    }

    {
        QMutexLocker locker(&m_connectWaitMutex);
        if (m_connectWait)
            m_connectWaitCondition.wait(&m_connectWaitMutex);
    }

    {
        QMutexLocker locker(&m_messageWaitMutex);
        if (!m_messageWait.isNull())
            m_messageWaitCondition.wait(&m_messageWaitMutex);
    }
    return true;
}

void MessageHandler::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "ERROR: socket error" << error;
}

void MessageHandler::connectedSocketError(QAbstractSocket::SocketError error)
{
    // We expect the remote host to close this connection so ignore the error
    if (error == QAbstractSocket::RemoteHostClosedError)
        return;

    qDebug() << "ERROR: connected socket error" << error;
}

void MessageHandler::incomingConnection(int socketDescriptor)
{
    ConnectionThread *thread = new ConnectionThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(socketError(QAbstractSocket::SocketError)),
            this, SLOT(connectedSocketError(QAbstractSocket::SocketError)));
    thread->start();

    QMutexLocker locker(&m_connectWaitMutex);
    if (m_connectWait) {
        m_connectWait = false;
        m_connectWaitCondition.wakeAll();
    }
}

void MessageHandler::handleMessageInternal(Message* msg,  const QHostAddress& address, quint16 port)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Receiving message" << *msg << "from" << address << "on" << port;
#else
    Q_UNUSED(port);
#endif

    handleMessage(msg, address);

    QMutexLocker locker(&m_messageWaitMutex);
    if (!m_messageWait.isNull() && address == m_messageWait) {
        m_messageWait = QHostAddress();
        m_messageWaitCondition.wakeAll();
    }
}

#include "messagehandler.moc"