#include "messagehandler.h"

#include <QtCore/QDebug>

#define DEBUG_MESSAGEHANDLER 0

MessageFilter::MessageFilter()
    : m_sessionId(QByteArray())
    , m_type(Message::Type(-1))
    , m_origin(QHostAddress::Any)
{
}

QByteArray MessageFilter::sessionId() const
{
    return m_sessionId;
}

void MessageFilter::setSessionId(const QByteArray& id)
{
    m_sessionId = id;
}

Message::Type MessageFilter::type() const
{
    return m_type;
}

void MessageFilter::setType(Message::Type type)
{
    m_type = type;
}

QHostAddress MessageFilter::origin() const
{
    return m_origin;
}

void MessageFilter::setOrigin(const QHostAddress& origin)
{
    m_origin = origin;
}

QDebug operator<<(QDebug d, const MessageFilter& filter)
{
    d.nospace() << "MessageFilter(" << filter.sessionId() << ", " << filter.type() << ", " << filter.origin().toIPv4Address() << ")";
    return d.space();
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
