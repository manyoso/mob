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

    bool sendMessage(Message* msg);
    void expectMessage();
    bool blockForMessage(unsigned long timeout = ULONG_MAX);
    QSharedPointer<Message> lastMessageReceived() const;

private slots:
    bool sendMessageInternal(Message* msg);
    void expectMessageInternal();
    bool waitForMessageInternal(unsigned long timeout);
    void handleMessage(QSharedPointer<Message>, const QHostAddress&);

private:
    QSharedPointer<MessageHandler> m_handler;
    QSharedPointer<Message> m_message;
    QThread* m_thread;
};

#endif // peer_h
