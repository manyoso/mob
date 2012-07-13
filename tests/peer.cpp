#include "peer.h"

#include "global.h"

#include <QtTest/QtTest>

Peer::Peer(quint16 readPort, quint16 writePort)
    : MessageServer(Global::firstIPv4Address("localhost"), readPort, writePort, 0)
{
    m_thread = new QThread(this);
    moveToThread(m_thread);
    m_thread->start();
}

Peer::~Peer()
{
    m_thread->quit();
    if (!m_thread->wait(1000)) {
        m_thread->terminate();
        m_thread->wait();
    }
}

bool Peer::sendMessage(const Message& msg, bool sync)
{
    bool r;
    QMetaObject::invokeMethod(this, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(const Message&, msg), Q_ARG(bool, sync));
    return r;
}

bool Peer::sendMessageInternal(const Message& msg, bool sync)
{
    return MessageServer::sendMessage(msg, QHostAddress::LocalHost, sync);
}
