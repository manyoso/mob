#ifndef messageserver_h
#define messageserver_h

#include "message.h"
#include "messagehandler.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkAddressEntry>

Q_DECLARE_METATYPE(QAbstractSocket::SocketError);
Q_DECLARE_METATYPE(QSharedPointer<Message>);
Q_DECLARE_METATYPE(QHostAddress);

class MessageServerPrivate;
class MessageServer : public QObject {
    Q_OBJECT
public:
    MessageServer(const QNetworkAddressEntry& address, quint16 port, QObject* parent);
    MessageServer(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent);
    virtual ~MessageServer();

    quint16 readPort() const;
    quint16 writePort() const;

    /*!
      \brief Sends the message to the specified address.
     * If sync is true it will block until the message is sent and transmitting
     * over the wire.
     */
     bool sendMessage(const Message& msg, const QHostAddress& address, bool sync = false);

    /*!
      \brief Installs a message handler for messages received from host.
     * Note, the message handler is passed as a shared pointer, but this class does
     * not store a reference to this shared pointer. Only one message handler for
     * a given host is installed at any one time.  If a message handler is installed
     * for QHostAddress::Any, then all messages will go exclusively to this handler
     * for instance.
     */
    void installMessageHandler(QSharedPointer<MessageHandler>, const QHostAddress& = QHostAddress::Any);

private:
    MessageServerPrivate* d;
};

#endif // messageserver_h
