#include "testmessage.h"

#include "peer.h"

#include <QtCore/QUuid>

#define INSTALL_CUSTOM_MESSAGE_FACTORY(messageType, messageFactory) \
    static MessageFactory s_##messageType##Factory \
        = Message::installMessageFactory((Message::Type)messageType, messageFactory);

static const quint8 GenericType1    = Message::NumberOfTypes;
static const quint8 GenericType2    = Message::NumberOfTypes + 1;
static const quint8 GenericType3    = Message::NumberOfTypes + 2;
static const quint8 GenericType4    = Message::NumberOfTypes + 3;
static const quint8 GenericType5    = Message::NumberOfTypes + 4;
static const quint8 GenericType6    = Message::NumberOfTypes + 5;
static const quint8 GenericType7    = Message::NumberOfTypes + 6;
static const quint8 GenericType8    = Message::NumberOfTypes + 7;
static const quint8 RawDataType     = Message::NumberOfTypes + 8;

class Generic : public Message {
    Q_OBJECT
public:
    Generic(quint8 type = GenericType1) : Message((Message::Type)type) {}
    static Message* createMessage() { return new Generic; }
};

INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType1, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType2, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType3, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType4, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType5, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType6, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType7, &Generic::createMessage);
INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType8, &Generic::createMessage);

class RawData : public Message {
    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data WRITE setData STORED false)
public:
    RawData() : Message((Message::Type)RawDataType) {}
    const QByteArray& data() const { return m_data; }
    void setData(const QByteArray& data) { m_data = data; }

    virtual bool serialize(QIODevice* device) const;
    virtual bool deserialize(QIODevice* device);

    static Message* createMessage() { return new RawData; }

private:
    QByteArray m_data;
};

INSTALL_CUSTOM_MESSAGE_FACTORY(RawDataType, &RawData::createMessage);

bool RawData::serialize(QIODevice* device) const
{
    quint32 size = m_data.size();
    if (device->write(reinterpret_cast<char*>(&size), sizeof(quint32)) == -1)
        return false;

    if (device->write(m_data) == -1)
        return false;

    return true;
}

bool RawData::deserialize(QIODevice* device)
{
    while(device->bytesAvailable() < 4)
        device->waitForReadyRead(-1);

    quint32 size = 0;
    if (device->read(reinterpret_cast<char*>(&size), sizeof(quint32)) < 4)
        return false;

    while (quint32(m_data.size()) < size) {
        m_data.append(device->readAll());
        device->waitForReadyRead(-1);
    }

    return true;
}

struct TestMessagePrivate {
};

void TestMessage::initTestCase()
{
    d = new TestMessagePrivate;
}

void TestMessage::cleanupTestCase()
{
    delete d;
    d = 0;
}

void TestMessage::testSendMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    QSharedPointer<MessageHandler> handler(new MessageHandler);
    peer2.installMessageHandler(handler, MessageFilter());

    Generic msg;
    QVERIFY(peer1.sendMessage(msg, false /*sync*/) == true);

    QVERIFY(handler->waitForMessage() == true);
    QVERIFY(handler->messageCount() == 1);
    QSharedPointer<Message> out = handler->dequeueMessage();
    QVERIFY(msg.type() == out->type());
}

void TestMessage::testSendLargeMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    QSharedPointer<MessageHandler> handler(new MessageHandler);
    peer2.installMessageHandler(handler, MessageFilter());

    QByteArray data;
    data.fill('X', 1024 * 1024);
    RawData msg;
    msg.setData(data);
    QVERIFY(peer1.sendMessage(msg, false /*sync*/) == true);

    QVERIFY(handler->waitForMessage() == true);
    QVERIFY(handler->messageCount() == 1);
    QSharedPointer<Message> out = handler->dequeueMessage();
    QVERIFY(out->type() == msg.type());
    if (out->type() == msg.type()) {
        QSharedPointer<RawData> raw = out.staticCast<RawData>();
        QVERIFY(raw->data() == msg.data());
    }
}

void TestMessage::testMessageWaitTimeout()
{
    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);
    QSharedPointer<MessageHandler> handler(new MessageHandler);
    peer2.installMessageHandler(handler, MessageFilter());
    QVERIFY(handler->waitForMessage(0) == false);
}

void TestMessage::testMessageFilter()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    const QUuid sessionId = QUuid::createUuid();

    // Full filter
    MessageFilter fullFilter;
    fullFilter.setSessionId(sessionId.toByteArray());
    fullFilter.setType(Message::Type(GenericType1));
    fullFilter.setOrigin(QHostAddress::LocalHost);
    QSharedPointer<MessageHandler> fullHandler(new MessageHandler);
    peer2.installMessageHandler(fullHandler, fullFilter);

    // With any type
    MessageFilter anyType = fullFilter;
    anyType.setType(Message::Type(-1));
    QSharedPointer<MessageHandler> anyTypeHandler(new MessageHandler);
    peer2.installMessageHandler(anyTypeHandler, anyType);

    // With any session id
    MessageFilter anySession = fullFilter;
    anySession.setSessionId(QByteArray());
    QSharedPointer<MessageHandler> anySessionHandler(new MessageHandler);
    peer2.installMessageHandler(anySessionHandler, anySession);

    // With any session id with any type
    MessageFilter anySessionAnyType = anyType;
    anySessionAnyType.setSessionId(QByteArray());
    QSharedPointer<MessageHandler> anySessionAnyTypeHandler(new MessageHandler);
    peer2.installMessageHandler(anySessionAnyTypeHandler, anySessionAnyType);

    Generic msg1(GenericType1);
    msg1.setSessionId(sessionId.toByteArray());
    QVERIFY(peer1.sendMessage(msg1) == true); // for full filter

    Generic msg2(GenericType2);
    msg2.setSessionId(sessionId.toByteArray());
    QVERIFY(peer1.sendMessage(msg2) == true); // for any type filter

    Generic msg3(GenericType1);
    QVERIFY(peer1.sendMessage(msg3) == true); // for any session filter

    Generic msg4(GenericType2);
    QVERIFY(peer1.sendMessage(msg4) == true); // for any session with any type filter

    QVERIFY(fullHandler->waitForMessage() == true);
    QVERIFY(fullHandler->messageCount() == 1);

    QVERIFY(anyTypeHandler->waitForMessage() == true);
    QVERIFY(anyTypeHandler->messageCount() == 1);

    QVERIFY(anySessionHandler->waitForMessage() == true);
    QVERIFY(anySessionHandler->messageCount() == 1);

    QVERIFY(anySessionAnyTypeHandler->waitForMessage() == true);
    QVERIFY(anySessionAnyTypeHandler->messageCount() == 1);
}

#include "testmessage.moc"
