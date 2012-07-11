#ifndef message_h
#define message_h

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtNetwork/QHostAddress>

//! \brief Used to register a factory method for a specific type of message.
class Message;
typedef Message* (*MessageFactory)();
#define INSTALL_MESSAGE_FACTORY(messageType, messageFactory) \
    static MessageFactory s_##messageType##Factory \
        = Message::installMessageFactory(Message::messageType, messageFactory);

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
        NodeInfo = 0
    };

    const static quint16 NumberOfTypes = NodeInfo + 1;
    static Message* createMessage(Message::Type);
    static Message* cloneMessage(const Message*);
    static MessageFactory installMessageFactory(Message::Type, MessageFactory);

    Type type() const { return m_type; }
    QHostAddress origin() const { return m_origin; }
    void setOrigin(const QHostAddress& origin) { m_origin = origin; }

    virtual bool serialize(QIODevice* device) const;
    virtual bool deserialize(QIODevice* device);

protected:
    Message(Type type) : m_type(type) {}
    friend QDataStream& operator<<(QDataStream&, const Message&);
    friend QDataStream& operator>>(QDataStream&, Message&);
    friend QDebug operator<<(QDebug, const Message&);
    void serialize(QTextStream& stream) const;
    void serialize(QDataStream& stream) const;
    void deserialize(QDataStream& stream);

private:
    Type m_type;
    QHostAddress m_origin;
};

QDataStream& operator<<(QDataStream&, const Message&);
QDataStream& operator>>(QDataStream&, Message&);
QDebug operator<<(QDebug, const Message &);

#endif // message_h
