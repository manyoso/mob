#include "peer.h"

#include "global.h"

#include <QtTest/QtTest>

PeerServer::PeerServer(quint16 readPort, quint16 writePort)
    : MessageServer(Global::firstIPv4Address("localhost"), readPort, writePort, 0)
{
}

bool PeerServer::sendMessageInternal(const Message& msg, bool sync)
{
    return MessageServer::sendMessage(msg, QHostAddress::LocalHost, sync);
}

Peer::Peer(quint16 readPort, quint16 writePort)
{
    m_messageServer = new PeerServer(readPort, writePort);
    m_thread = new QThread(this);
    m_messageServer->moveToThread(m_thread);
    m_thread->start();
}

Peer::~Peer()
{
    m_messageServer->deleteLater();
    m_thread->quit();
    if (!m_thread->wait(1000)) {
        m_thread->terminate();
        m_thread->wait();
    }
}

quint16 Peer::readPort() const
{
    return m_messageServer->readPort();
}

quint16 Peer::writePort() const
{
    return m_messageServer->writePort();
}

bool Peer::sendMessage(const Message& msg, bool sync)
{
    bool r;
    QMetaObject::invokeMethod(m_messageServer, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(const Message&, msg), Q_ARG(bool, sync));
    return r;
}

void Peer::installMessageHandler(QSharedPointer<MessageHandler> handler, const MessageFilter& filter)
{
    return m_messageServer->installMessageHandler(handler, filter);
}
