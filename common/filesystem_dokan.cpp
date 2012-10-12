#include "filesystem.h"

#include "remotefileops.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

FileSystem::FileSystem(FileOps* ops)
    : QThread(0)
    , m_fileOps(ops)
    , m_rootString("")
    , m_started(false)
    , d(new FileSystemPrivate(this))
{
}

FileSystem::~FileSystem()
{
    stop();
    delete d;
    d = 0;
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
    m_mutex.lock();
    m_started = true;
    m_waitCondition.wakeAll();
    m_mutex.unlock();

    exec();
}
