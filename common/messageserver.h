#ifndef messageserver_h
#define messageserver_h

#include "message.h"

#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class NetworkServer;
class QThread;

Q_DECLARE_METATYPE(QAbstractSocket::SocketError);
Q_DECLARE_METATYPE(QSharedPointer<Message>);
Q_DECLARE_METATYPE(QHostAddress);

class MessageThread : public QThread {
    Q_OBJECT
public:
    virtual ~MessageThread();
signals:
    void socketError(QAbstractSocket::SocketError socketError);
    void incomingConnection(QSharedPointer<MessageThread>, const QHostAddress&);
    void receivedMessage(QSharedPointer<Message>, const QHostAddress&);

protected:
    virtual void run();

private:
    friend class MessageServer;
    MessageThread(int socketDescriptor);
    void readSocket(QTcpSocket* socket);

    int m_socketDescriptor;
    bool m_firstRead;
    quint16 m_typeOfMessage;
    quint32 m_sizeOfMessage;
    QHostAddress m_address;
    quint16 m_port;
};

Q_DECLARE_METATYPE(QSharedPointer<MessageThread>);

class MessageServer : public QTcpServer {
    Q_OBJECT
public:
    MessageServer(const QNetworkAddressEntry& address, quint16 port, QObject* parent);
    MessageServer(const QNetworkAddressEntry& address, quint16 readPort, quint16 writePort, QObject* parent);
    virtual ~MessageServer();

    bool isRunning() const;

    quint16 readPort() const { return m_readPort; }
    quint16 writePort() const { return m_writePort; }

    bool sendMessage(Message* msg, const QHostAddress& address, bool sync = false);
    void expectMessage(const QHostAddress& address);
    bool waitForMessage(unsigned long timeout = ULONG_MAX);

signals:
    void incomingConnection(QSharedPointer<MessageThread>, const QHostAddress&);
    void receivedMessage(QSharedPointer<Message>, const QHostAddress&);

protected:
    virtual void incomingConnection(int);

private slots:
    void messageThreadFinished();
    void socketError(QAbstractSocket::SocketError);
    void connectedSocketError(QAbstractSocket::SocketError);
    void receivedMessageInternal(QSharedPointer<Message>, const QHostAddress&);

private:
    void init();

    QNetworkAddressEntry m_networkAddress;
    quint16 m_readPort;
    quint16 m_writePort;
    QTcpSocket *m_tcpSocket;

    bool m_connectWait;
    QMutex m_connectWaitMutex;
    QWaitCondition m_connectWaitCondition;

    QHostAddress m_messageWait;
    QMutex m_messageWaitMutex;
    QWaitCondition m_messageWaitCondition;

    QSet< QSharedPointer<MessageThread> > m_threads;
};

#endif // messageserver_h
