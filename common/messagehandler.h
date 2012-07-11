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

    QByteArray messageId() const;
    void setMessageId(const QByteArray&);

    Message::Type messageType() const;
    void setMessageType(Message::Type);

    QHostAddress address() const;
    void setAddress(const QHostAddress&);

private:
    QByteArray m_messageId;
    Message::Type m_messageType;
    QHostAddress m_address;
};

inline bool operator==(const MessageFilter &f1, const MessageFilter &f2)
{
    return f1.messageId() == f2.messageId()
        && f1.messageType() == f2.messageType()
        && f1.address() == f2.address();
}

inline uint qHash(const MessageFilter &key)
{
    return qHash(key.messageId()) ^ qHash(key.messageType()) ^ qHash(key.address().toIPv4Address());
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
