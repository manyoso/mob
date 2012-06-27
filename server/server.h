#ifndef server_h
#define server_h

#include "node.h"

#include <QtNetwork/QNetworkAddressEntry>

class QUdpSocket;

class Server : public Node {
    Q_OBJECT
public:
    Server(const QNetworkAddressEntry& address, QObject* parent);

    QNetworkAddressEntry networkAddress() const { return m_networkAddress; }

private slots:
    void broadcast();
    void processPendingDatagrams();

private:
    QNetworkAddressEntry m_networkAddress;
    QUdpSocket *m_udpSocket;
};

#endif // server_h
