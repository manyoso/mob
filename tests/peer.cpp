#include "peer.h"

Peer::Peer(quint16 readPort, quint16 writePort)
    : MessageServer(Global::firstIPv4Address("localhost"), readPort, writePort, 0)
{
    m_handler = QSharedPointer<MessageHandler>(new MessageHandler());
    installMessageHandler(m_handler, MessageFilter());

    m_thread = new QThread(this);
    moveToThread(m_thread);
    m_thread->start();
}

Peer::~Peer()
{
    m_handler.clear();
    m_message.clear();
    m_thread->quit();
    if (!m_thread->wait(1000)) {
        m_thread->terminate();
        m_thread->wait();
    }
}

bool Peer::sendMessage(const Message& msg)
{
    bool r;
    QMetaObject::invokeMethod(this, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(const Message&, msg));
    return r;
}

bool Peer::waitForMessage(unsigned long timeout)
{
    return m_handler->waitForMessage(timeout);
}

QSharedPointer<Message> Peer::lastMessageReceived() const
{
    return m_handler->dequeueMessage();
}

bool Peer::sendMessageInternal(const Message& msg)
{
    return MessageServer::sendMessage(msg, QHostAddress::LocalHost, false);
}
