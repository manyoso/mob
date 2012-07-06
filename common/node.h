#ifndef node_h
#define node_h

#include "message.h"

#include <QtNetwork/QHostAddress>

class FileOperations;

class NodeInfo : public Message {
    Q_OBJECT
    Q_PROPERTY(QHostAddress address READ address WRITE setAddress)
    Q_PROPERTY(bool scheduler READ scheduler WRITE setScheduler)
public:
    NodeInfo() : Message(Message::NodeInfo), m_scheduler(false) {}
    QHostAddress address() const { return m_address; }
    void setAddress(const QHostAddress& address) { m_address = address; }

    bool scheduler() const { return m_scheduler; }
    void setScheduler(bool scheduler) { m_scheduler = scheduler; }

    static Message* createMessage() { return new NodeInfo; }

private:
    QHostAddress m_address;
    bool m_scheduler;
};

INSTALL_MESSAGE_FACTORY(NodeInfo, &NodeInfo::createMessage);

class Node {
public:
    Node(bool isLocal, const QHostAddress& address);
    ~Node();

    //! \brief Returns whether this node is the scheduler
    bool scheduler() const;

    //! \brief Returns the host address of this node
    QHostAddress address() const { return m_address; }

    //! \brief Returns the file operations of this node
    FileOperations* fileOperations() const { return m_fileOps; }

private:
    QHostAddress m_address;
    FileOperations* m_fileOps;
};

#endif // node_h
