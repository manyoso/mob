#include "server.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>

#define _SCHEDULER_PORT_ 9876

static Node* s_scheduler = 0;

Server::Server(const QNetworkAddressEntry& address, bool isScheduler, QObject* parent)
    : MessageHandler(_SCHEDULER_PORT_, parent)
    , Node(address.ip())
    , m_networkAddress(address)
{
    m_udpSocket = new QUdpSocket(this);

    if (!m_udpSocket->bind(!isScheduler ? address.ip() : QHostAddress::Any, _SCHEDULER_PORT_, QUdpSocket::DontShareAddress)) {
        qDebug() << "ERROR: Could not exclusively bind to the port" << _SCHEDULER_PORT_ << "on this machine needed for scheduler!";
        exit(1);
    }

    if (!isScheduler) {
        s_scheduler = this;
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(broadcast()));
        timer->start(5000);
        broadcast();
    } else {
        connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    }
}

Server::~Server()
{
    qDeleteAll(m_nodes);
    m_nodes.clear();
}

Node* Server::scheduler() const
{
    return s_scheduler;
}

bool Server::isScheduler() const
{
    return this == s_scheduler;
}

void Server::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QHostAddress peerAddress;
        quint16 peerPort = 0;
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &peerAddress, &peerPort);
        qDebug() << "Received datagram:" << datagram.data() << "from" << peerAddress << "on" << peerPort;
        if (!m_nodes.contains(peerAddress)) {
            // Establish a TCP connection and write to it saying that the scheduler sees you
            Node* node = new Node(peerAddress);
            m_nodes.insert(peerAddress, node);

            sendMessage(HostInfo(address()), node);
        }
    }
}

void Server::broadcast()
{
    Q_ASSERT(!isScheduler());
    qDebug() << "Broadcast datagram: on" << networkAddress().broadcast();
    m_udpSocket->writeDatagram(0, 0, networkAddress().broadcast(), _SCHEDULER_PORT_);
}
