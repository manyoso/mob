#ifndef server_h
#define server_h

#include "node.h"
#include "messagehandler.h"

#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAddressEntry>

class QUdpSocket;

class Server;
extern Server* server();

class Server : public MessageHandler, public Node {
    Q_OBJECT
public:
    Server(const QNetworkAddressEntry& address, bool isScheduler, QObject* parent);
    virtual ~Server();

    QNetworkAddressEntry networkAddress() const { return m_networkAddress; }

protected:
    virtual void handleMessage(QSharedPointer<Message>, const QHostAddress&);

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
