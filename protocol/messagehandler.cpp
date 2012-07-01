#include "messagehandler.h"

#include "node.h"

#include <QtCore/QCoreApplication>
#define DEBUG_MESSAGEHANDLER 0


MessageHandler::MessageHandler(const QNetworkAddressEntry& address, quint16 port, QObject* parent)
    : QObject(parent)
    , m_networkAddress(address)
    , m_port(port)
{
    // Listens for new connections
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "ERROR: Could not start the TCP server!";
        exit(1);
    }

    // Listens for and reads incoming messages and writes outgoing messages
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
}

bool MessageHandler::sendMessage(const Message& msg, const QHostAddress& address)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Sending message" << msg << "to" << address << "on" << m_port;
#endif

    m_tcpSocket->connectToHost(address, m_port, QIODevice::WriteOnly);
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
    return true;
}

void MessageHandler::handleNewConnection()
{
    QTcpSocket* connectedSocket = m_tcpServer->nextPendingConnection();
    Q_ASSERT(connectedSocket);
    connect(connectedSocket, SIGNAL(disconnected()), connectedSocket, SLOT(deleteLater()));
    connect(connectedSocket, SIGNAL(readyRead()), this, SLOT(readConnectedSocket()));
    connect(connectedSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectedSocketError(QAbstractSocket::SocketError)));
}

void MessageHandler::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "ERROR: socket error" << error;
}

void MessageHandler::readConnectedSocket()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    quint8 type;
    QDataStream stream(socket);
    stream >> type;

    switch ((Message::Type)type) {
    case Message::Generic:
        {
            Message msg;
            stream >> msg;
            handleMessageInternal(&msg, socket);
            break;
        }
    case Message::NodeInfo:
        {
            NodeInfo msg;
            stream >> msg;
            handleMessageInternal(&msg, socket);
            break;
        }
    default:
        qDebug() << "ERROR: unrecognized message sent" << type << "!";
        QCoreApplication::exit(1);
    }
}

void MessageHandler::handleMessageInternal(Message* msg, QTcpSocket* socket)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Receiving message" << *msg << "from" << socket->peerAddress() << "on" << socket->peerPort();
#endif

    handleMessage(msg, socket->peerAddress());
}

void MessageHandler::connectedSocketError(QAbstractSocket::SocketError error)
{
    // We expect the remote host to close this connection so ignore the error
    if (error == QAbstractSocket::RemoteHostClosedError)
        return;

    qDebug() << "ERROR: connected socket error" << error;
}
