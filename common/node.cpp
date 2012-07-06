#include "node.h"

#include "global.h"
#include "localfile.h"
#include "remotefile.h"

Node::Node(bool isLocal, const QHostAddress& address)
    : m_address(address)
{
    if (isLocal)
        m_fileOps = new LocalFile(this);
    else
        m_fileOps = new RemoteFile(this);
}

Node::~Node()
{
    delete m_fileOps;
    m_fileOps = 0;
}

bool Node::scheduler() const
{
    return this == Global::scheduler();
}