#include "node.h"

#include "filesystem.h"
#include "global.h"
#include "localfile.h"
#include "remotefile.h"

Node::Node(bool isLocal, const QHostAddress& address)
    : m_isLocal(isLocal)
    , m_address(address)
{
    if (isLocal)
        m_fileOps = new LocalFile(this);
    else
        m_fileOps = new RemoteFile(this);

    m_fileSystem = new FileSystem(this);
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
    if (!m_fileSystem->isRunning())
        return;

    Q_ASSERT(!isLocal());
    if (isLocal())
        return;

    if (!m_fileSystem->wait(1000)) {
        m_fileSystem->terminate();
        m_fileSystem->wait();
    }
}
