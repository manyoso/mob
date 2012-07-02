#ifndef messagehandler_h
#define messagehandler_h

#include "protocol.h"

#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class NetworkServer;
class QThread;

class MessageHandler : public QTcpServer {
    Q_OBJECT
public:
    MessageHandler(const QNetworkAddressEntry& address, quint16 port, QObject* parent);
    MessageHandler(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent);
    virtual ~MessageHandler();

    bool isRunning() const;

    quint16 readPort() const { return m_readPort; }
    quint16 writePort() const { return m_writePort; }

    bool sendMessage(const Message& msg, const QHostAddress& address);
    void expectMessage(const QHostAddress& address);
    bool waitForMessage();

protected:
    //! \brief Reimplemented from QTcpServer.
    virtual void incomingConnection(int socketDescriptor);

    //! \brief Will be called on another thread.
    virtual void handleMessage(Message* msg, const QHostAddress& address) = 0;

private slots:
    void socketError(QAbstractSocket::SocketError);
    void connectedSocketError(QAbstractSocket::SocketError);

private:
    void init();
    friend class ConnectionThread;
    void handleMessageInternal(Message*, QTcpSocket*);

    QNetworkAddressEntry m_networkAddress;
    quint16 m_readPort;
    quint16 m_writePort;
    QTcpSocket *m_tcpSocket;

    QHostAddress m_waitingForMessage;
    QMutex m_waitMutex;
    QWaitCondition m_waitCondition;
};

#endif // messagehandler_h
