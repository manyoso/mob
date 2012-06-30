#include "filesystem.h"

#include "server.h"
#include "fileoperations.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define DEBUG_FILESYSTEM 0

struct FileSystemPrivate {};

FileSystem::FileSystem(QObject* parent)
    : QThread(parent)
    , d(new FileSystemPrivate)
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
