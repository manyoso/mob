#include "testmessage.h"

#include "peer.h"

#define INSTALL_CUSTOM_MESSAGE_FACTORY(messageType, messageFactory) \
    static MessageFactory s_##messageType##Factory \
        = Message::installMessageFactory((Message::Type)messageType, messageFactory);

static const quint8 GenericType = Message::NumberOfTypes + 1;
class Generic : public Message {
    Q_OBJECT
public:
    Generic() : Message((Message::Type)GenericType) {}
    static Message* createMessage() { return new Generic; }
};

INSTALL_CUSTOM_MESSAGE_FACTORY(GenericType, &Generic::createMessage);

static const quint8 RawDataType = Message::NumberOfTypes + 2;
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

void TestMessage::sendMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    Generic msg;
    QVERIFY(peer1.sendMessage(msg) == true);
    QVERIFY(peer2.waitForMessage() == true);
    QVERIFY(msg.type() == peer2.lastMessageReceived()->type());
}

void TestMessage::sendLargeMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    QByteArray data;
    data.fill('X', 1024 * 1024);
    RawData msg;
    msg.setData(data);
    QVERIFY(peer1.sendMessage(msg) == true);
    QVERIFY(peer2.waitForMessage() == true);

    QSharedPointer<Message> out = peer2.lastMessageReceived();
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
    QVERIFY(peer2.waitForMessage(0) == false);
}

#include "testmessage.moc"
