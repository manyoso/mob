#include "filesystem.h"

#include "remotefileops.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

struct FileSystemPrivate
{
    FileSystemPrivate(FileOps* ops)
    {
        m_fileOps = ops;
    }

    FileOps* m_fileOps;
};

FileSystem::FileSystem(FileOps* ops)
    : QThread(0)
    , d(new FileSystemPrivate(ops))
{
}

FileSystem::~FileSystem()
{
    stop();
    delete d;
}

void FileSystem::stop()
{
    exit(0);
    if (!wait(5000)) {
        terminate();
        wait();
    }
}

void FileSystem::run()
{
    exec();
}
