#include "messagehandler.h"

#include "node.h"

MessageHandler::MessageHandler(quint16 port, QObject* parent)
    : QObject(parent)
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
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
}

bool MessageHandler::sendMessage(const Message& message, Node* node)
{
    if (!node) {
        qDebug() << "ERROR: Sending message because the node is null!";
        return false;
    }

    m_tcpSocket->connectToHost(node->address(), m_port, QIODevice::WriteOnly);
    if (!m_tcpSocket->waitForConnected(1000)) {
        qDebug() << "ERROR: Sending message" << m_tcpSocket->error();
        return false;
    }

    QByteArray bytes;
    QDataStream stream;
    stream << message;
    stream >> bytes;
    if (m_tcpSocket->write(bytes) == -1) {
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

void MessageHandler::readSocket()
{
    qDebug() << "MessageHandler::readSocket";
}

void MessageHandler::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "MessageHandler::socketError" << error;
}

void MessageHandler::readConnectedSocket()
{
    qDebug() << "MessageHandler::readConnectedSocket";
}

void MessageHandler::connectedSocketError(QAbstractSocket::SocketError error)
{
    qDebug() << "MessageHandler::connectedSocketError" << error;
}
