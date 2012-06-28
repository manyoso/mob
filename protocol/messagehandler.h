#ifndef messagehandler_h
#define messagehandler_h

#include "protocol.h"

#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class Node;

class MessageHandler : public QObject {
    Q_OBJECT
public:
    MessageHandler(quint16 port, QObject* parent);

    quint16 port() const { return m_port; }

    bool sendMessage(const Message& message, Node* node);

private slots:
    void handleNewConnection();
    void readSocket();
    void socketError(QAbstractSocket::SocketError);
    void readConnectedSocket();
    void connectedSocketError(QAbstractSocket::SocketError);

private:
    quint16 m_port;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
};

#endif // messagehandler_h
