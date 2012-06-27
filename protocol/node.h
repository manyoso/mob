#ifndef node_h
#define node_h

#include "protocol.h"

#include <QtCore/QObject>

class Node : public QObject {
    Q_OBJECT
public:
    Node(const QHostAddress& address, QObject* parent);

    //! \brief Returns the host address of this node
    QHostAddress address() const { return m_address; }

    //! \brief Dispatches the message to the host address represented by this node
    void dispatchMessage(const Message&);

private:
    QHostAddress m_address;
};

#endif // node_h
