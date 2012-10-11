#ifndef node_h
#define node_h

#include "message.h"

#include <QtNetwork/QHostAddress>

class FileOps;
class FileSystem;

class NodeInfo : public Message {
    Q_OBJECT
    Q_PROPERTY(quint32 address READ address WRITE setAddress)
    Q_PROPERTY(bool scheduler READ scheduler WRITE setScheduler)
public:
    NodeInfo() : Message(Message::NodeInfo), m_scheduler(false) {}
    quint32 address() const { return m_address; }
    void setAddress(quint32 address) { m_address = address; }

    bool scheduler() const { return m_scheduler; }
    void setScheduler(bool scheduler) { m_scheduler = scheduler; }

    static Message* createMessage() { return new NodeInfo; }

private:
    quint32 m_address;
    bool m_scheduler;
};

INSTALL_MESSAGE_FACTORY(NodeInfo, &NodeInfo::createMessage);

class Node {
public:
    Node(bool isLocal, const QHostAddress& address);
    ~Node();

    //! \brief Returns whether this node is the scheduler.
    bool scheduler() const;

    //! \brief Returns whether this node represents the local node.
    bool isLocal() const{ return m_isLocal; }

    //! \brief Returns the host address of this node.
    QHostAddress address() const { return m_address; }

    //! \brief Returns the file operations of this node.
    FileOps* fileOperations() const { return m_fileOps; }

    void startFileSystem();
    void stopFileSystem();

private:
    bool m_isLocal;
    QHostAddress m_address;
    FileOps* m_fileOps;
    FileSystem* m_fileSystem;
};

#endif // node_h
