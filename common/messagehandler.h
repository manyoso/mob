#ifndef messagehandler_h
#define messagehandler_h

#include "message.h"

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QWaitCondition>

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
    friend class MessageServer;
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

#endif // messagehandler_h
