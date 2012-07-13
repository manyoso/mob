#ifndef peer_h
#define peer_h

#include <QtCore/QThread>
#include <QtTest/QtTest>

#include "message.h"
#include "messageserver.h"

class Peer : public MessageServer {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort);
    virtual ~Peer();

    bool sendMessage(const Message& msg, bool sync = true);

private slots:
    bool sendMessageInternal(const Message& msg, bool sync);

private:
    QThread* m_thread;
};

#endif // peer_h
