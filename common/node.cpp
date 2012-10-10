#include "node.h"

#include "filesystem.h"
#include "global.h"
#include "localfileops.h"
#include "remotefileops.h"

Node::Node(bool isLocal, const QHostAddress& address)
    : m_isLocal(isLocal)
    , m_address(address)
    , m_fileOps(0)
    , m_fileSystem(0)
{
    if (isLocal)
        m_fileOps = new LocalFileOps(address);
    else {
        m_fileOps = new RemoteFileOps(address);
        m_fileSystem = new FileSystem(static_cast<RemoteFileOps*>(m_fileOps));
    }
}

Node::~Node()
{
    stopFileSystem();

    delete m_fileSystem;
    m_fileSystem = 0;

    delete m_fileOps;
    m_fileOps = 0;
}

bool Node::scheduler() const
{
    return this == Global::scheduler();
}

void Node::startFileSystem()
{
    Q_ASSERT(!isLocal());
    if (isLocal())
        return;
    m_fileSystem->start();
}

void Node::stopFileSystem()
{
    if (!m_fileSystem || !m_fileSystem->isRunning())
        return;

    Q_ASSERT(!isLocal());
    if (isLocal())
        return;

    if (!m_fileSystem->wait(1000)) {
        m_fileSystem->terminate();
        m_fileSystem->wait();
    }
}
