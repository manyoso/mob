#ifndef messagehandler_h
#define messagehandler_h

#include "protocol.h"

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class MessageHandler : public QObject {
    Q_OBJECT
public:
    MessageHandler(const QNetworkAddressEntry& address, quint16 port, QObject* parent);

    quint16 port() const { return m_port; }

    bool sendMessage(const Message& msg, const QHostAddress& address);

protected:
    virtual void handleMessage(Message* msg, const QHostAddress& address) = 0;

private slots:
    void handleNewConnection();
    void socketError(QAbstractSocket::SocketError);
    void readConnectedSocket();
    void handleMessageInternal(Message*, QTcpSocket*);
    void connectedSocketError(QAbstractSocket::SocketError);

private:
    QNetworkAddressEntry m_networkAddress;
    quint16 m_port;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
};

#endif // messagehandler_h
