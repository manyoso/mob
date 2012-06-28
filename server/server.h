#ifndef server_h
#define server_h

#include "node.h"

#include <QtNetwork/QNetworkAddressEntry>

class QUdpSocket;

class Server : public Node {
    Q_OBJECT
public:
    Server(const QNetworkAddressEntry& address, bool isScheduler, QObject* parent);

    QNetworkAddressEntry networkAddress() const { return m_networkAddress; }

    bool isScheduler() const { return m_isScheduler; }

private slots:
    void broadcast();
    void processPendingDatagrams();

private:
    bool m_isScheduler;
    QNetworkAddressEntry m_networkAddress;
    QUdpSocket *m_udpSocket;
};

#endif // server_h
