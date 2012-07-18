#include "filesystem.h"

#include "remotefileops.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

struct FileSystemPrivate
{
    FileSystemPrivate(RemoteFileOps* ops)
    {
        m_fileOps = ops;
    }

    RemoteFileOps* m_fileOps;
};

FileSystem::FileSystem(RemoteFileOps* ops)
    : QThread(0)
    , d(new FileSystemPrivate(ops))
{
}

FileSystem::~FileSystem()
{
    delete d;
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
