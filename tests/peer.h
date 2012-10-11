#ifndef peer_h
#define peer_h

#include <QtCore/QThread>
#include <QtTest/QtTest>

#include "message.h"
#include "messageserver.h"

class PeerServer : public MessageServer {
    Q_OBJECT
public:
    PeerServer(quint16 readPort, quint16 writePort);

private slots:
    bool sendMessageInternal(const Message& msg, bool sync);
};


class Peer : public QObject {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort);
    virtual ~Peer();

    quint16 readPort() const;
    quint16 writePort() const;
    bool sendMessage(const Message& msg, bool sync = true);
    void installMessageHandler(QSharedPointer<MessageHandler>, const MessageFilter&);

private:
    QThread* m_thread;
    PeerServer* m_messageServer;
};

#endif // peer_h
