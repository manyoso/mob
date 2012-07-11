#include "messagehandler.h"

#include <QtCore/QDebug>
#include <QtCore/QTime>

#define DEBUG_MESSAGEHANDLER 0

MessageFilter::MessageFilter()
    : m_messageType(Message::Type(-1))
{
}

QByteArray MessageFilter::messageId() const
{
    return m_messageId;
}

void MessageFilter::setMessageId(const QByteArray& id)
{
    m_messageId = id;
}

Message::Type MessageFilter::messageType() const
{
    return m_messageType;
}

void MessageFilter::setMessageType(Message::Type type)
{
    m_messageType = type;
}

QHostAddress MessageFilter::address() const
{
    return m_address;
}

void MessageFilter::setAddress(const QHostAddress& address)
{
    m_address = address;
}

MessageHandler::MessageHandler()
    : m_connectWait(false)
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::expectMessage(const QHostAddress& address)
{
    {
        QMutexLocker locker(&m_connectWaitMutex);
        m_connectWait = true;
    }

    {
        QMutexLocker locker(&m_messageWaitMutex);
        m_messageWait = address;
    }
}

bool MessageHandler::waitForMessage(unsigned long timeout)
{
    QTime time;
    if (timeout != ULONG_MAX)
        time.start();

    {
        QMutexLocker locker(&m_connectWaitMutex);
        if (m_connectWait && !m_connectWaitCondition.wait(&m_connectWaitMutex, timeout)) {
            m_connectWait = false;
            return false;
        }
    }

    {
        QMutexLocker locker(&m_messageWaitMutex);
        if (!m_messageWait.isNull() && !m_messageWaitCondition.wait(&m_messageWaitMutex, timeout == ULONG_MAX ? timeout : timeout - time.elapsed())) {
            m_messageWait = QHostAddress();
            return false;
        }
    }
    return true;
}

void MessageHandler::incomingConnectionInternal(const QHostAddress& address)
{
    emit incomingConnection(address);

    QMutexLocker locker(&m_connectWaitMutex);
    if (m_connectWait) {
        m_connectWait = false;
        m_connectWaitCondition.wakeAll();
    }
}

void MessageHandler::receivedMessageInternal(QSharedPointer<Message> msg,  const QHostAddress& address)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Receiving message" << *msg << "from" << address;
#endif

    emit receivedMessage(msg, address);

    QMutexLocker locker(&m_messageWaitMutex);
    if (!m_messageWait.isNull() && address == m_messageWait) {
        m_messageWait = QHostAddress();
        m_messageWaitCondition.wakeAll();
    }
}
