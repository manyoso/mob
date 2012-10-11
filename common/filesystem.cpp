#include "filesystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

// Store and wrap the constructed mountPoint string and root string as light weight
// as possible by using QByteArray and QLatin1String.  These are going to be very
// important to keep efficient as they will be used everywhere.
const QLatin1String& FileSystem::mountPoint()
{
    static QByteArray mount = QDir::tempPath().append(QDir::separator()).append(QCoreApplication::applicationName()).toLatin1();
    static QLatin1String mountString = QLatin1String(mount.constData());
    return mountString;
}

static QByteArray* s_root = new QByteArray("");
static QLatin1String* s_rootString = new QLatin1String(s_root->constData());
const QLatin1String& FileSystem::root()
{
    return *s_rootString;
}

void FileSystem::setRoot(const QLatin1String& root)
{
    *s_root = QByteArray(root.latin1());
    *s_rootString = QLatin1String(s_root->constData());
}

QByteArray FileSystem::absolutePath(const char* path)
{
    QByteArray p = QByteArray::fromRawData(path, qstrlen(path));
    if (s_root->isEmpty())
        return p;
    else
        return *s_root + p;
}
