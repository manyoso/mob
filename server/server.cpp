#include "server.h"

#include "filesystem.h"
#include "global.h"

#include <QtCore/QCoreApplication>
#include <QtNetwork/QUdpSocket>

#define DEBUG_SERVER 0
#define _SCHEDULER_PORT_ 9876

static Server* s_server = 0;
Server* server()
{
    return s_server;
}

Server::Server(const QNetworkAddressEntry& address, bool isScheduler, QObject* parent)
    : MessageHandler(address, _SCHEDULER_PORT_, parent)
    , Node(true /*isLocal*/, address.ip())
    , m_networkAddress(address)
    , m_broadcastTimer(0)
{
    s_server = this;

    FileSystem* fileSystem = new FileSystem(this);
    connect(fileSystem, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));
    connect(fileSystem, SIGNAL(terminated()), QCoreApplication::instance(), SLOT(quit()));
    fileSystem->start();

    m_udpSocket = new QUdpSocket(this);

    if (!m_udpSocket->bind(!isScheduler ? address.ip() : QHostAddress::Any, _SCHEDULER_PORT_, QUdpSocket::DontShareAddress)) {
        qDebug() << "ERROR: Could not exclusively bind to the port" << _SCHEDULER_PORT_ << "on this machine needed for scheduler!";
        exit(1);
    }

    if (!isScheduler) {
        m_broadcastTimer = new QTimer(this);
        connect(m_broadcastTimer, SIGNAL(timeout()), this, SLOT(broadcast()));
        m_broadcastTimer->start(5000);
        broadcast();
    } else {
        Global::setScheduler(this);
        connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    }
}

Server::~Server()
{
    qDeleteAll(m_nodes);
    m_nodes.clear();
}

Node* Server::currentJob() const
{
    if (m_nodes.isEmpty())
        return 0;
    return m_nodes.values().first();
}

void Server::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QHostAddress peerAddress;
        quint16 peerPort = 0;
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &peerAddress, &peerPort);
#if DEBUG_SERVER
        qDebug() << "Receiving connection request from" << peerAddress << "on" << peerPort;
#endif
        if (!m_nodes.contains(peerAddress)) {
            // Establish a TCP connection and write to it saying that the scheduler sees you
            Node* node = new Node(false /*isLocal*/, peerAddress);
            m_nodes.insert(peerAddress, node);
            NodeInfo info;
            info.setAddress(address());
            info.setScheduler(scheduler());
            sendMessage(&info, node->address());
        }
    }
}

void Server::broadcast()
{
    Q_ASSERT(!scheduler());
#if DEBUG_SERVER
    qDebug() << "Broadcasting to scheduler" << networkAddress().broadcast() << "on" << _SCHEDULER_PORT_;
#endif
    m_udpSocket->writeDatagram(0, 0, networkAddress().broadcast(), _SCHEDULER_PORT_);
}

void Server::handleMessage(Message* msg, const QHostAddress& address)
{
    Q_UNUSED(address);

    // The first message we receive will be an info message from the scheduler
    if (!scheduler() && m_broadcastTimer->isActive() && msg->type() == Message::NodeInfo) {
        m_broadcastTimer->stop();
        NodeInfo* nodeInfo = static_cast<NodeInfo*>(msg);
        Q_ASSERT(nodeInfo->scheduler());
        Global::setScheduler(new Node(false /*isLocal*/, nodeInfo->address()));
#if DEBUG_SERVER
        qDebug() << "Found scheduler at" << nodeInfo->address();
#endif
    }
}
