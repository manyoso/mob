#include "remotefile.h"

#define DEBUG_REMOTEFILE 0

bool RemoteFile::getattr(const QString& path, QFileInfo* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file getattr" << path;
#endif
    return true;
}

bool RemoteFile::readlink(const QString& path, QTextStream& buffer) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file readlink" << path;
#endif
    return true;
}

bool RemoteFile::create(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file create" << path << perm;
#endif
    return true;
}

bool RemoteFile::mkdir(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file mkdir" << path << perm;
#endif
    return true;
}

bool RemoteFile::unlink(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file unlink" << path;
#endif
    return true;
}

bool RemoteFile::rmdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file rmdir" << path;
#endif
    return true;
}

bool RemoteFile::symlink(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file symlink" << path1 << path2;
#endif
    return true;
}

bool RemoteFile::rename(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file rename" << path1 << path2;
#endif
    return true;
}

bool RemoteFile::link(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file link" << path1 << path2;
#endif
    return true;
}

bool RemoteFile::chmod(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file chmod" << path << perm;
#endif
    return true;
}

bool RemoteFile::chown(const QString& path, quint32 uid, quint32 gid) const
{
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file chown" << path << uid << gid;
#endif
    return true;
}

bool RemoteFile::truncate(const QString& path, qint64 size) const
{
    Q_UNUSED(path);
    Q_UNUSED(size);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file truncate" << path << size;
#endif
    return true;
}

bool RemoteFile::open(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file open" << path;
#endif
    return true;
}

bool RemoteFile::read(const QString& path, QByteArray *buffer, size_t size, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file read" << path << size << offset;
#endif
    return true;
}

bool RemoteFile::write(const QString& path, const QByteArray &data, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file write" << path << data << offset;
#endif
    return true;
}

bool RemoteFile::flush(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file flush" << path;
#endif
    return true;
}

bool RemoteFile::release(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file release" << path;
#endif
    return true;
}

bool RemoteFile::fsync(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file fsync" << path;
#endif
    return true;
}

bool RemoteFile::opendir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file opendir" << path;
#endif
    return true;
}

bool RemoteFile::readdir(const QString& path, QDir* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file readdir" << path;
#endif
    return true;
}

bool RemoteFile::releasedir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file releasedir" << path;
#endif
    return true;
}

bool RemoteFile::fsyncdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file fsyncdir" << path;
#endif
    return true;
}

bool RemoteFile::utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const
{
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
#if DEBUG_LOCALFILE
    qDebug() << "Remote file utime" << path << acc << mod;
#endif
    return true;
}
