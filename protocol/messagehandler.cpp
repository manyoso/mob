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
};

ConnectionThread::ConnectionThread(int socketDescriptor, MessageHandler* parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
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

#if DEBUG_MESSAGEHANDLER
    qDebug() << "Starting new connection thread with" << tcpSocket.peerAddress() << "on" << tcpSocket.peerPort();
#endif

    while (tcpSocket.state() == QAbstractSocket::ConnectedState) {
        if (tcpSocket.bytesAvailable())
            readSocket(&tcpSocket);
#if DEBUG_MESSAGEHANDLER
        qDebug() << "Waiting for socket read in state" << tcpSocket.state();
#endif
        tcpSocket.waitForReadyRead();
    }
}

void ConnectionThread::readSocket(QTcpSocket* socket)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Reading socket with" << socket->bytesAvailable() << "bytesAvailable";
#endif

    quint8 type;
    QDataStream stream(socket);
    stream >> type;

    MessageHandler* handler = qobject_cast<MessageHandler*>(parent());

    switch ((Message::Type)type) {
    case Message::Generic:
        {
            Message msg;
            stream >> msg;
            handler->handleMessageInternal(&msg, socket);
            break;
        }
    case Message::NodeInfo:
        {
            NodeInfo msg;
            stream >> msg;
            handler->handleMessageInternal(&msg, socket);
            break;
        }
    default:
        qDebug() << "ERROR: unrecognized message sent" << type << "!";
        QCoreApplication::exit(1);
    }
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

bool MessageHandler::sendMessage(const Message& msg, const QHostAddress& address)
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot send a message because the TCP server is not running!";
        return false;
    }

#if DEBUG_MESSAGEHANDLER
    qDebug() << "Sending message" << msg << "to" << address << "on" << m_writePort;
#endif

    m_tcpSocket->connectToHost(address, m_writePort, QIODevice::WriteOnly);
    if (!m_tcpSocket->waitForConnected(1000)) {
        qDebug() << "ERROR: Sending message" << m_tcpSocket->error();
        return false;
    }

    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << (quint8)msg.type(); // Write the type which will be used to infer size as well
    stream << msg;
    int b = m_tcpSocket->write(bytes);
    if (b == -1) {
        qDebug() << "ERROR: Sending message could not write the message to the socket!";
        return false;
    }

    m_tcpSocket->disconnectFromHost();
    m_tcpSocket->waitForDisconnected();
    return true;
}

void MessageHandler::expectMessage(const QHostAddress& address)
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot expect message because the TCP server is not running!";
        return;
    }

    QMutexLocker locker(&m_waitMutex);
    m_waitingForMessage = address;
}

bool MessageHandler::waitForMessage()
{
    if (!isRunning()) {
        qDebug() << "ERROR: Cannot wait for message because The TCP server is not running!";
        return false;
    }

    waitForNewConnection(-1, 0);

    QMutexLocker locker(&m_waitMutex);
    if (m_waitingForMessage.isNull())
        return true;

    m_waitCondition.wait(&m_waitMutex);
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
}

void MessageHandler::handleMessageInternal(Message* msg, QTcpSocket* socket)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Receiving message" << *msg << "from" << socket->peerAddress() << "on" << socket->peerPort();
#endif

    handleMessage(msg, socket->peerAddress());

    QMutexLocker locker(&m_waitMutex);
    if (!m_waitingForMessage.isNull() && socket->peerAddress() == m_waitingForMessage) {
        m_waitingForMessage = QHostAddress();
        m_waitCondition.wakeAll();
    }
}

#include "messagehandler.moc"
