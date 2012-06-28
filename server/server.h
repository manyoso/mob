#ifndef server_h
#define server_h

#include "node.h"
#include "messagehandler.h"

#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAddressEntry>

class QUdpSocket;

class Server : public MessageHandler, public Node {
    Q_OBJECT
public:
    Server(const QNetworkAddressEntry& address, bool isScheduler, QObject* parent);
    virtual ~Server();

    bool isScheduler() const;
    Node* scheduler() const;

    QNetworkAddressEntry networkAddress() const { return m_networkAddress; }

protected:
    virtual void handleMessage(Message* message, const QHostAddress& address);

private slots:
    void broadcast();
    void processPendingDatagrams();

private:
    QNetworkAddressEntry m_networkAddress;
    QUdpSocket* m_udpSocket;
    QHash<QHostAddress, Node*> m_nodes;
    QTimer* m_broadcastTimer;
};

#endif // server_h
