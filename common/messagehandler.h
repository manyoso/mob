#ifndef messagehandler_h
#define messagehandler_h

#include "message.h"

#include <QtCore/QMutex>
#include <QtCore/QObject>
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

    void expectMessage(const QHostAddress&);
    bool waitForMessage(unsigned long timeout = ULONG_MAX);

signals:
    void incomingConnection(const QHostAddress&);
    void receivedMessage(QSharedPointer<Message>, const QHostAddress&);

private:
    friend class MessageServerPrivate;
    void incomingConnectionInternal(const QHostAddress&);
    void receivedMessageInternal(QSharedPointer<Message>,  const QHostAddress&);

private:
    bool m_connectWait;
    QMutex m_connectWaitMutex;
    QWaitCondition m_connectWaitCondition;

    QHostAddress m_messageWait;
    QMutex m_messageWaitMutex;
    QWaitCondition m_messageWaitCondition;
};

Q_DECLARE_METATYPE(QSharedPointer<Message>);
Q_DECLARE_METATYPE(QHostAddress);

#endif // messagehandler_h
