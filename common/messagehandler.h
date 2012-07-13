#ifndef messagehandler_h
#define messagehandler_h

#include "message.h"

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QSharedPointer>
#include <QtCore/QWaitCondition>

/*!
 * \brief The MessageFilter class is used to filter messages given a set of rules.
 *
 * Order of matching is from most specific to least specific defined as
 * follows: session id (S), message type (T), origin (O). Thus the table
 * below lists the possible filter matching rules in order of preference.
 * Note that (_) is used as the wildcard marker and if a given message does
 * not specify a session id, then only the last four rules will be evaluated.
 *
 * 1)  S T O
 * 2)  S T _
 * 3)  S _ O
 * 4)  S _ _
 * 5)  _ T O
 * 6)  _ T _
 * 7)  _ _ O
 * 8)  _ _ _
 */
class MessageFilter {
public:
    MessageFilter();

    QByteArray sessionId() const;
    void setSessionId(const QByteArray&);

    Message::Type type() const;
    void setType(Message::Type);

    QHostAddress origin() const;
    void setOrigin(const QHostAddress&);

private:
    QByteArray m_sessionId;
    Message::Type m_type;
    QHostAddress m_origin;
};

inline bool operator==(const MessageFilter &f1, const MessageFilter &f2)
{
    return f1.sessionId() == f2.sessionId()
        && f1.type() == f2.type()
        && f1.origin() == f2.origin();
}

inline bool operator!=(const MessageFilter &f1, const MessageFilter &f2)
{
    return !(f1 == f2);
}

inline uint qHash(const MessageFilter &key)
{
    return qHash(key.sessionId()) ^ qHash(key.type()) ^ qHash(key.origin().toIPv4Address());
}

QDebug operator<<(QDebug, const MessageFilter&);

class MessageHandler : public QObject {
    Q_OBJECT
public:
    MessageHandler();
    virtual ~MessageHandler();

    quint32 messageCount() const;
    QSharedPointer<Message> dequeueMessage();
    bool waitForMessage(unsigned long timeout = ULONG_MAX);

private:
    friend class MessageServerPrivate;
    void receivedMessageInternal(QSharedPointer<Message>);

private:
    QQueue< QSharedPointer<Message> > m_messages;
    QMutex m_messageWaitMutex;
    QWaitCondition m_messageWaitCondition;
};

#endif // messagehandler_h
