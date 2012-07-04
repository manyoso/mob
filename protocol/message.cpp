#include "message.h"

#include "node.h"

QLatin1String messageTypeToString(Message::Type type)
{
    switch (type) {
    case Message::Generic: return QLatin1String("Generic");
    case Message::NodeInfo: return QLatin1String("NodeInfo");
    case Message::RawData: return QLatin1String("RawData");
    default: return QLatin1String("");
    }
}

Message* Message::createMessage(Message::Type type)
{
    switch (type) {
    case Message::Generic:
        return new Message(Generic);
    case Message::NodeInfo:
        return new ::NodeInfo;
    case Message::RawData:
        return new ::RawData;
    default:
        qDebug() << "ERROR: unrecognized message type" << type << "!";
        return 0;
    }
}

void Message::serialize(QTextStream& stream) const
{
    stream << "Type=" << messageTypeToString(m_type);
}

void Message::serialize(QDataStream& stream) const
{
    stream << (quint8)m_type;
}

void Message::deserialize(QDataStream& stream)
{
    quint8 type;
    stream >> type;
    m_type = (Message::Type)type;
}

NodeInfo::NodeInfo(Node* node)
    : Message(Message::NodeInfo)
    , m_address(node->address())
{
}

void NodeInfo::serialize(QTextStream& stream) const
{
    Message::serialize(stream);
    stream << " Address=" << m_address.toIPv4Address();
    stream << " Scheduler=" << (m_isScheduler ? "true" : "false");
}

void NodeInfo::serialize(QDataStream& stream) const
{
    Message::serialize(stream);
    stream << (quint32)m_address.toIPv4Address();
    stream << m_isScheduler;
}

void NodeInfo::deserialize(QDataStream& stream)
{
    Message::deserialize(stream);
    quint32 address;
    stream >> address;
    m_address = QHostAddress(address);
    bool scheduler;
    stream >> scheduler;
    m_isScheduler = scheduler;
}

RawData::RawData(const QByteArray& data)
    : Message(Message::RawData)
    , m_data(data)
{
}

void RawData::serialize(QTextStream& stream) const
{
    Message::serialize(stream);
    stream << " DataSize=" << m_data.size();
}

void RawData::serialize(QDataStream& stream) const
{
    Message::serialize(stream);
    stream << m_data;
}

void RawData::deserialize(QDataStream& stream)
{
    Message::deserialize(stream);
    stream >> m_data;
}

QDataStream& operator<<(QDataStream& stream, const Message& msg)
{
    msg.serialize(stream);
    return stream;
}

QDataStream& operator>>(QDataStream& stream, Message& msg)
{
    msg.deserialize(stream);
    return stream;
}

QDebug operator<<(QDebug d, const Message &msg)
{
    QString data;
    QTextStream stream(&data);
    msg.serialize(stream);
    d.nospace() << "Message(" << data << ")";
    return d.space();
}

