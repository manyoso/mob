#include "messagehandler.h"

#include <QtCore/QDebug>

#define DEBUG_MESSAGEHANDLER 0

MessageFilter::MessageFilter()
    : m_messageId(QByteArray())
    , m_messageType(Message::Type(-1))
    , m_address(QHostAddress::Any)
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
{
}

MessageHandler::~MessageHandler()
{
}

quint32 MessageHandler::messageCount() const
{
    return m_messages.count();
}

QSharedPointer<Message> MessageHandler::dequeueMessage()
{
    if (m_messages.isEmpty())
        return QSharedPointer<Message>(0);

    return m_messages.dequeue();
}

bool MessageHandler::waitForMessage(unsigned long timeout)
{
    QMutexLocker locker(&m_messageWaitMutex);
    if (m_messages.isEmpty() && !m_messageWaitCondition.wait(&m_messageWaitMutex, timeout))
        return false;

    Q_ASSERT(!m_messages.isEmpty());
    return !m_messages.isEmpty();
}

void MessageHandler::receivedMessageInternal(QSharedPointer<Message> msg)
{
#if DEBUG_MESSAGEHANDLER
    qDebug() << "Receiving message" << *msg << "from" << msg->origin();
#endif

    QMutexLocker locker(&m_messageWaitMutex);
    m_messages.enqueue(msg);
    m_messageWaitCondition.wakeAll();
}
