#include "filesystem.h"

#include "server.h"
#include "fileoperations.h"
#include "node.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

struct FileSystemPrivate
{
    FileSystemPrivate(Node* node)
    {
        m_node = node;
    }

    Node* m_node;
};

FileSystem::FileSystem(Node* node)
    : QThread(0)
    , d(new FileSystemPrivate(node))
{
}

FileSystem::~FileSystem()
{
    delete d;
}

Node* FileSystem::node() const
{
    Q_ASSERT(d);
    return d->m_node;
}

QString FileSystem::mountPoint() const
{
    QString mount = QDir::tempPath();
    mount.append(QCoreApplication::applicationName());
    return mount;
}

void FileSystem::run()
{
    exec();
}
