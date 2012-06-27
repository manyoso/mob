#include "node.h"

Node::Node(const QHostAddress& address, QObject* parent)
    : QObject(parent)
    , m_address(address)
{
}
