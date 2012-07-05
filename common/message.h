#ifndef message_h
#define message_h

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtNetwork/QHostAddress>

/*!
 * \brief This is the base class for all messages.
 * All methods marked with Q_PROPERTY will be automatically serialized/deserialized.
 * For properties that should be serialized/deserialized directly from a QIODevice
 * rather than from a QDataStream you can mark them with 'STORAGE false' and
 * re-implement the virual serialize and deserialize functions to read from and
 * write to the QIODevice directly.
 */
class Message : public QObject {
    Q_OBJECT
    Q_ENUMS(Type)
public:
    enum Type {
        Generic = 0,
        NodeInfo,
        RawData
    };

    static Message* createMessage(Message::Type);
    static Message* cloneMessage(const Message*);

    Message(Type type = Generic) : m_type(type) {}
    Type type() const { return m_type; }

    virtual bool serialize(QIODevice* device);
    virtual bool deserialize(QIODevice* device);

protected:
    friend QDataStream& operator<<(QDataStream&, const Message&);
    friend QDataStream& operator>>(QDataStream&, Message&);
    friend QDebug operator<<(QDebug, const Message&);
    void serialize(QTextStream& stream) const;
    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);

private:
    Type m_type;
};

class NodeInfo : public Message {
    Q_OBJECT
    Q_PROPERTY(QHostAddress address READ address WRITE setAddress)
    Q_PROPERTY(bool scheduler READ scheduler WRITE setScheduler)
public:
    NodeInfo() : Message(Message::NodeInfo), m_scheduler(false) {}
    QHostAddress address() const { return m_address; }
    void setAddress(const QHostAddress& address) { m_address = address; }

    bool scheduler() const { return m_scheduler; }
    void setScheduler(bool scheduler) { m_scheduler = scheduler; }

private:
    QHostAddress m_address;
    bool m_scheduler;
};

class RawData : public Message {
    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data WRITE setData STORED false)
public:
    RawData() : Message(Message::RawData) {}
    const QByteArray& data() const { return m_data; }
    void setData(const QByteArray& data) { m_data = data; }

    virtual bool serialize(QIODevice* device);
    virtual bool deserialize(QIODevice* device);

private:
    QByteArray m_data;
};

QDataStream& operator<<(QDataStream&, const Message&);
QDataStream& operator>>(QDataStream&, Message&);
QDebug operator<<(QDebug, const Message &);

#endif // message_h
