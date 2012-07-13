#ifndef messageserver_h
#define messageserver_h

#include "message.h"
#include "messagehandler.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkAddressEntry>

class MessageServerPrivate;
class MessageServer : public QObject {
    Q_OBJECT
public:
    MessageServer(const QNetworkAddressEntry& address, quint16 port, QObject* parent);
    MessageServer(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent);
    virtual ~MessageServer();

    //! \brief Returns the read port that this server reads on
    quint16 readPort() const;

    //! \brief Returns the write port that this server writes on
    quint16 writePort() const;

    /*!
     * \brief Sends the message to the specified address.
     * If sync is true it will block until the message is sent and transmitting
     * over the wire.
     */
     bool sendMessage(const Message& msg, const QHostAddress& address, bool sync = true);

    /*!
     * \brief Installs a message handler for messages received from host.
     * Note, the message handler is passed as a shared pointer, but this class does
     * not store a reference to this shared pointer. Only one message handler for
     * a given filter is installed at any one time.  If a message handler is installed
     * for a given filter, then all messages will go exclusively to this handler
     * for instance.  The filter matching will match from most specific to least
     * specific. See the MessageFilter class documentation for details.
     */
    void installMessageHandler(QSharedPointer<MessageHandler>, const MessageFilter&);

private:
    MessageServerPrivate* d;
};

#endif // messageserver_h
