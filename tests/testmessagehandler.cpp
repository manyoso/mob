#include "testmessagehandler.h"

#include <QtCore/QThread>

#include "global.h"
#include "message.h"
#include "messagehandler.h"

class Peer : public MessageHandler {
    Q_OBJECT
public:
    Peer(quint16 readPort, quint16 writePort)
        : MessageHandler(Global::firstIPv4Address("localhost"), readPort, writePort, 0)
    {
        m_thread = new QThread(this);
        moveToThread(m_thread);
        m_thread->start();
    }

    virtual ~Peer()
    {
        m_thread->quit();
        if (!m_thread->wait(1000)) {
            m_thread->terminate();
            m_thread->wait();
        }
    }

    bool sendMessage(const Message& msg)
    {
        bool r;
        QMetaObject::invokeMethod(this, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(Message, msg));
        return r;
    }

    void expectMessage()
    {
        QMetaObject::invokeMethod(this, "expectMessageInternal", Qt::BlockingQueuedConnection);
    }

    bool blockForMessage()
    {
        bool r;
        QMetaObject::invokeMethod(this, "waitForMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r));
        return r;
    }

private slots:
    bool sendMessageInternal(const Message& msg)
    {
        return MessageHandler::sendMessage(msg, QHostAddress::LocalHost, false);
    }

    void expectMessageInternal()
    {
        MessageHandler::expectMessage(QHostAddress::LocalHost);
    }

    bool waitForMessageInternal()
    {
        return MessageHandler::waitForMessage();
    }

protected:
    virtual void handleMessage(Message* msg, const QHostAddress& address)
    {
        Q_UNUSED(msg);
        Q_UNUSED(address);
    }

private:
    QThread* m_thread;
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
    Peer peer1(1111, 2222);
    Peer peer2(2222, 1111);
    peer2.expectMessage();

    Message msg;
    QVERIFY(peer1.sendMessage(msg) == true);
    QVERIFY(peer2.blockForMessage() == true);
}

void TestMessageHandler::sendLargeMessage()
{
    // Thiago said that over 1460 bytes is the key to whether the tcp socket will buffer so we need to check that...
    Peer peer1(1111, 2222);
    Peer peer2(2222, 1111);
    peer2.expectMessage();

    QByteArray data;
    data.fill('X', 1024 * 1024);
    RawData msg(data);
    QVERIFY(peer1.sendMessage(msg) == true);
    QVERIFY(peer2.blockForMessage() == true);
}

#include "testmessagehandler.moc"
