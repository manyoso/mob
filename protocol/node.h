#ifndef node_h
#define node_h

#include <QtNetwork/QHostAddress>

class FileOperations;

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
