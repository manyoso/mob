#include "server.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>

#define _SCHEDULER_PORT_ 9876

Server::Server(const QNetworkAddressEntry& address, QObject* parent)
    : Node(address.ip(), parent)
    , m_networkAddress(address)
{
    m_udpSocket = new QUdpSocket(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcast()));
    timer->start(5000);

    bool success = m_udpSocket->bind(_SCHEDULER_PORT_, QUdpSocket::DontShareAddress);
    if (!success) {
        qDebug() << "ERROR: Could not exclusively bind to the port 9876 on this machine so could not connect to scheduler!";
        exit(1);
    }

    connect(m_udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    broadcast();
}

void Server::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Received datagram:" << datagram.data();
    }
}

void Server::broadcast()
{
    QByteArray datagram = address().toString().toAscii();
    qDebug() << "Broadcast datagram:" << datagram.data();
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), networkAddress().broadcast(), _SCHEDULER_PORT_);
}
