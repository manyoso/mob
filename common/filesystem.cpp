#include "filesystem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

// Store and wrap the constructed mountPoint string and root string as light weight
// as possible by using QByteArray and QLatin1String.  These are going to be very
// important to keep efficient as they will be used everywhere.
const QLatin1String& FileSystem::mountPoint() const
{
    static QByteArray mount = QDir::tempPath().append(QDir::separator()).append(QCoreApplication::applicationName()).toLatin1();
    static QLatin1String mountString = QLatin1String(mount.constData());
    return mountString;
}

const QLatin1String& FileSystem::root() const
{
    return m_rootString;
}

void FileSystem::setRoot(const QLatin1String& root)
{
    m_root = QByteArray(root.latin1());
    m_rootString = QLatin1String(m_root.constData());
}

QByteArray FileSystem::absolutePath(const char* path) const
{
    QByteArray p = QByteArray::fromRawData(path, qstrlen(path));
    if (m_root.isEmpty())
        return p;
    else
        return m_root + p;
}

void FileSystem::startFileSystem()
{
    QMutexLocker locker(&m_mutex);
    start();
    if (!m_started) {
        bool rc = m_waitCondition.wait(&m_mutex);
        Q_ASSERT(rc);
    }
    Q_ASSERT(m_started);
}
