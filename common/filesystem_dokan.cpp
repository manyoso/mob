#include "filesystem.h"

#include "remotefileops.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

FileSystem::FileSystem(FileOps* ops)
    : QThread(0)
    , m_fileOps(ops)
    , m_rootString("")
{
}

FileSystem::~FileSystem()
{
    stop();
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
