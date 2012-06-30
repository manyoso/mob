#ifndef protocol_h
#define protocol_h

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtNetwork/QHostAddress>

class Message {
public:
    enum Type {
        Generic = 0,
        NodeInfo
    };

    Message(Type type = Generic) : m_type(type) {}
    Type type() const { return m_type; }

protected:
    friend QDataStream& operator<<(QDataStream&, const Message&);
    friend QDataStream& operator>>(QDataStream&, Message&);
    friend QDebug operator<<(QDebug, const Message&);
    virtual void serialize(QTextStream& stream) const;
    virtual void serialize(QDataStream& stream) const;
    virtual void deserialize(QDataStream& stream);

private:
    Type m_type;
};

class Node;
class NodeInfo : public Message {
public:
    NodeInfo(Node* node);
    QHostAddress address() const { return m_address; }
    bool isScheduler() const { return m_isScheduler; }

protected:
    NodeInfo() : Message(Message::NodeInfo) {}
    virtual void serialize(QTextStream& stream) const;
    virtual void serialize(QDataStream& stream) const;
    virtual void deserialize(QDataStream& stream);

private:
    QHostAddress m_address;
    bool m_isScheduler;
};

QDataStream& operator<<(QDataStream&, const Message&);
QDataStream& operator>>(QDataStream&, Message&);
QDebug operator<<(QDebug, const Message &);

#endif // protocol_h
