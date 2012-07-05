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
        m_message = 0;
        m_thread = new QThread(this);
        moveToThread(m_thread);
        m_thread->start();
    }

    virtual ~Peer()
    {
        delete m_message;
        m_message = 0;

        m_thread->quit();
        if (!m_thread->wait(1000)) {
            m_thread->terminate();
            m_thread->wait();
        }
    }

    bool sendMessage(Message* msg)
    {
        bool r;
        QMetaObject::invokeMethod(this, "sendMessageInternal", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, r), Q_ARG(Message*, msg));
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

    Message* lastMessageReceived()
    {
        return m_message;
    }

private slots:
    bool sendMessageInternal(Message* msg)
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
        QVERIFY(address == QHostAddress::LocalHost);
        m_message = Message::cloneMessage(msg);
    }

private:
    Message* m_message;
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
    QVERIFY(peer1.sendMessage(&msg) == true);
    QVERIFY(peer2.blockForMessage() == true);
    QVERIFY(msg.type() == peer2.lastMessageReceived()->type());
}

void TestMessageHandler::sendLargeMessage()
{
    Peer peer1(1111, 2222);
    Peer peer2(2222, 1111);
    peer2.expectMessage();

    QByteArray data;
    data.fill('X', 1024 * 1024);
    RawData msg;
    msg.setData(data);
    QVERIFY(peer1.sendMessage(&msg) == true);
    QVERIFY(peer2.blockForMessage() == true);

    Message* out = peer2.lastMessageReceived();
    QVERIFY(out->type() == msg.type());
    if (out->type() == msg.type()) {
        RawData* raw = static_cast<RawData*>(out);
        QVERIFY(raw->data() == msg.data());
    }
}

#include "testmessagehandler.moc"
