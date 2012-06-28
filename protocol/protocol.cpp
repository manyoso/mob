#include "protocol.h"

QLatin1String messageTypeToString(Message::Type type)
{
    switch (type) {
    case Message::Generic: return QLatin1String("Generic");
    case Message::HostInfo: return QLatin1String("HostInfo");
    default: return QLatin1String("");
    }
}

void Message::serialize(QTextStream& stream) const
{
    stream << "Type=" << messageTypeToString(m_type);
}

void Message::serialize(QDataStream& stream) const
{
    stream << (int)m_type;
}

void Message::deserialize(QDataStream& stream)
{
    int type;
    stream >> type;
    m_type = (Message::Type)type;
}

void HostInfo::serialize(QTextStream& stream) const
{
    Message::serialize(stream);
    stream << " Address=" << m_address.toIPv4Address();
}

void HostInfo::serialize(QDataStream& stream) const
{
    Message::serialize(stream);
    stream << (int)m_address.toIPv4Address();
}

void HostInfo::deserialize(QDataStream& stream)
{
    Message::deserialize(stream);
    int address;
    stream >> address;
    m_address = QHostAddress(address);
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

