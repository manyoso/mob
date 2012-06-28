#ifndef node_h
#define node_h

#include "protocol.h"

#include <QtNetwork/QHostAddress>

class Node {
public:
    Node(const QHostAddress& address);

    //! \brief Returns the host address of this node
    QHostAddress address() const { return m_address; }

private:
    QHostAddress m_address;
};

#endif // node_h
