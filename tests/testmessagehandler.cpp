#include "testmessagehandler.h"

#include "global.h"
#include "messagehandler.h"
#include "protocol.h"

class Peer : public MessageHandler {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort, QObject* parent)
        : MessageHandler(Global::firstIPv4Address("localhost"), readPort, writePort, parent)
    {
    }

protected:
    virtual void handleMessage(Message* msg, const QHostAddress& address)
    {
        Q_UNUSED(msg);
        Q_UNUSED(address);
    }
};

struct TestMessageHandlerPrivate {
};

void TestMessageHandler::initTestCase()
{
    d = new TestMessageHandlerPrivate;
}

void TestMessageHandler::cleanupTestCase()
{
    delete d;
    d = 0;
}

void TestMessageHandler::sendMessage()
{
    Peer peer1(1111, 2222, this);
    Peer peer2(2222, 1111, this);

    Message msg;
    peer2.expectMessage(QHostAddress::LocalHost);
    peer1.sendMessage(msg, QHostAddress::LocalHost);
    QVERIFY(peer2.waitForMessage() == true);

    // Thiago said that over 1460 bytes is the key to whether the tcp socket will buffer so we need to check that...
}

#include "testmessagehandler.moc"
