#ifndef messagehandler_h
#define messagehandler_h

#include "message.h"

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QSharedPointer>
#include <QtCore/QWaitCondition>

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

inline uint qHash(const MessageFilter &key)
{
    return qHash(key.sessionId()) ^ qHash(key.type()) ^ qHash(key.origin().toIPv4Address());
}

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
