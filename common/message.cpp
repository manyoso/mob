#include "message.h"

#include <QMetaProperty>

QHash<Message::Type, MessageFactory>* factories()
{
    static QHash<Message::Type, MessageFactory>* s_factories = 0;
    if (!s_factories)
        s_factories = new QHash<Message::Type, MessageFactory>();
    return s_factories;
}

Message* Message::createMessage(Message::Type type)
{
    Q_ASSERT(factories()->contains(type));
    if (!factories()->contains(type)) {
        qDebug() << "ERROR: unrecognized message type" << type << "!";
        return 0;
    }

    MessageFactory factory = factories()->value(type);
    Message* message = (*factory)();
    if (message)
        message->m_type = type;
    return message;
}

MessageFactory Message::installMessageFactory(Message::Type type, MessageFactory factory)
{
    factories()->insert(type, factory);
    return factory;
}

Message* Message::cloneMessage(const Message* messageToClone)
{
    Message* message = Message::createMessage(messageToClone->type());

    const QMetaObject* object = message->metaObject();
    const QMetaObject* objectToClone = messageToClone->metaObject();
    Q_ASSERT(object == objectToClone);
    if (object != objectToClone) {
        delete message;
        return 0;
    }

    for(int i = objectToClone->propertyOffset(); i < objectToClone->propertyCount(); ++i) {
        QMetaProperty propertyToClone = objectToClone->property(i);
        QVariant value = propertyToClone.read(messageToClone);
        if (!propertyToClone.write(message, value)) {
            Q_ASSERT(false);
            delete message;
            return 0;
        }
    }

    return message;
}

void Message::serialize(QTextStream& stream) const
{
    const QMetaObject* object = metaObject();
    stream << object->className() << "(";
    bool firstProperty = true;
    while (object && QLatin1String(object->className()) != QLatin1String("QObject")) {
        for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
            if (!firstProperty)
                stream << ", ";
            firstProperty = false;
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
        object = object->superClass();
    }
    stream << ")";
}

void Message::serialize(QDataStream& stream) const
{
    const QMetaObject* object = metaObject();
    while (object && QLatin1String(object->className()) != QLatin1String("QObject")) {
        for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
            QMetaProperty property = object->property(i);
            if (property.isStored(this))
                stream << property.read(this);
        }
        object = object->superClass();
    }
}

void Message::deserialize(QDataStream& stream)
{
    const QMetaObject* object = metaObject();
    while (object && QLatin1String(object->className()) != QLatin1String("QObject")) {
        for(int i = object->propertyOffset(); i < object->propertyCount(); ++i) {
            QMetaProperty property = object->property(i);
            if (property.isStored(this))
                property.write(this, stream);
        }
        object = object->superClass();
    }
}

bool Message::serialize(QIODevice*) const
{
    // no-op
    return true;
}

bool Message::deserialize(QIODevice*)
{
    // no-op
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

QDebug operator<<(QDebug d, const Message& msg)
{
    QString data;
    QTextStream stream(&data);
    msg.serialize(stream);
    d.nospace() << data;
    return d.space();
}

