#ifndef peer_h
#define peer_h

#include <QtCore/QThread>
#include <QtTest/QtTest>

#include "global.h"
#include "message.h"
#include "messagehandler.h"

class Peer : public MessageHandler {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort);
    virtual ~Peer();

    bool sendMessage(Message* msg);
    void expectMessage();
    bool blockForMessage(unsigned long timeout = ULONG_MAX);
    QSharedPointer<Message> lastMessageReceived() const;

private slots:
    bool sendMessageInternal(Message* msg);
    void expectMessageInternal();
    bool waitForMessageInternal(unsigned long timeout);

protected:
    virtual void handleMessage(QSharedPointer<Message>, const QHostAddress&);

private:
    QSharedPointer<Message> m_message;
    QThread* m_thread;
};

#endif // peer_h
