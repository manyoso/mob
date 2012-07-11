#ifndef peer_h
#define peer_h

#include <QtCore/QThread>
#include <QtTest/QtTest>

#include "global.h"
#include "message.h"
#include "messageserver.h"

class Peer : public MessageServer {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort);
    virtual ~Peer();

    bool sendMessage(const Message& msg);
    bool waitForMessage(unsigned long timeout = ULONG_MAX);
    QSharedPointer<Message> lastMessageReceived() const;

private slots:
    bool sendMessageInternal(const Message& msg);

private:
    QSharedPointer<MessageHandler> m_handler;
    QSharedPointer<Message> m_message;
    QThread* m_thread;
};

#endif // peer_h
