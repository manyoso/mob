#include "peer.h"

Peer::Peer(quint16 readPort, quint16 writePort)
    : MessageHandler(Global::firstIPv4Address("localhost"), readPort, writePort, 0)
{
    m_thread = new QThread(this);
    moveToThread(m_thread);
    m_thread->start();

    connect(this, SIGNAL(receivedMessage(QSharedPointer<Message>, const QHostAddress&)),
            this, SLOT(handleMessage(QSharedPointer<Message>, const QHostAddress&)), Qt::DirectConnection);
}

Peer::~Peer()
{
    m_message.clear();
    m_thread->quit();
    if (!m_thread->wait(1000)) {
        m_thread->terminate();
        m_thread->wait();
    }
}

bool Peer::sendMessage(Message* msg)
{
    bool r;
    QMetaObject::invokeMethod(this, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(Message*, msg));
    return r;
}

void Peer::expectMessage()
{
    QMetaObject::invokeMethod(this, "expectMessageInternal", Qt::BlockingQueuedConnection);
}

bool Peer::blockForMessage(unsigned long timeout)
{
    bool r;
    QMetaObject::invokeMethod(this, "waitForMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(unsigned long, timeout));
    return r;
}

QSharedPointer<Message> Peer::lastMessageReceived() const
{
    return m_message;
}

bool Peer::sendMessageInternal(Message* msg)
{
    return MessageHandler::sendMessage(msg, QHostAddress::LocalHost, false);
}

void Peer::expectMessageInternal()
{
    MessageHandler::expectMessage(QHostAddress::LocalHost);
}

bool Peer::waitForMessageInternal(unsigned long timeout)
{
    return MessageHandler::waitForMessage(timeout);
}

void Peer::handleMessage(QSharedPointer<Message> msg, const QHostAddress& address)
{
    QVERIFY(address == QHostAddress::LocalHost);
    m_message = msg;
}