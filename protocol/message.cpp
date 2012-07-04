#include "message.h"

#include <QMetaProperty>

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
    const QMetaObject* object = metaObject();
    stream << object->className() << "(";
    for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
        if (i != object->propertyOffset())
            stream << ", ";
        QMetaProperty property = object->property(i);
        QString name = property.name();
        name.replace(0, 1, name.at(0).toUpper());
        stream << name;
        stream << ":";
        QString value = property.read(this).toString();
        if (value.length() > 10) {
            value.truncate(10);
            value.append("...");
        }
        if (value.isEmpty())
            value = "0";
        stream << value;
    }
    stream << ")";
}

void Message::serialize(QDataStream& stream) const
{
    stream << (quint8)m_type;
    const QMetaObject* object = metaObject();
    for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
        QMetaProperty property = object->property(i);
        if (property.isStored(this))
            stream << property.read(this);
    }
}

void Message::deserialize(QDataStream& stream)
{
    quint8 type;
    stream >> type;
    m_type = (Message::Type)type;
    const QMetaObject* object = metaObject();
    for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
        QMetaProperty property = object->property(i);
        if (property.isStored(this))
            property.write(this, stream);
    }
}

bool Message::serialize(QIODevice*)
{
    // no-op
    return true;
}

bool Message::deserialize(QIODevice*)
{
    // no-op
    return true;
}

bool RawData::serialize(QIODevice* device)
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
    d.nospace() << data;
    return d.space();
}

