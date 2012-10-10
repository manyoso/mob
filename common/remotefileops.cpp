#include "remotefileops.h"

#define DEBUG_REMOTEFILEOPS 0

bool RemoteFileOps::getattr(const QLatin1String& path, FileInfo* info)
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file getattr" << path;
#endif
    return true;
}

bool RemoteFileOps::readlink(const QLatin1String& path, QTextStream& buffer)
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file readlink" << path;
#endif
    return true;
}

bool RemoteFileOps::create(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file create" << path << perm;
#endif
    return true;
}

bool RemoteFileOps::mkdir(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file mkdir" << path << perm;
#endif
    return true;
}

bool RemoteFileOps::unlink(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file unlink" << path;
#endif
    return true;
}

bool RemoteFileOps::rmdir(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file rmdir" << path;
#endif
    return true;
}

bool RemoteFileOps::symlink(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file symlink" << path1 << path2;
#endif
    return true;
}

bool RemoteFileOps::rename(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file rename" << path1 << path2;
#endif
    return true;
}

bool RemoteFileOps::link(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file link" << path1 << path2;
#endif
    return true;
}

bool RemoteFileOps::chmod(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file chmod" << path << perm;
#endif
    return true;
}

bool RemoteFileOps::chown(const QLatin1String& path, quint32 uid, quint32 gid)
{
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file chown" << path << uid << gid;
#endif
    return true;
}

bool RemoteFileOps::truncate(const QLatin1String& path, qint64 size)
{
    Q_UNUSED(path);
    Q_UNUSED(size);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file truncate" << path << size;
#endif
    return true;
}

bool RemoteFileOps::open(const QLatin1String& path, qint32 flags, quint64* fh)
{
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file open" << path;
#endif
    return true;
}

bool RemoteFileOps::read(const QLatin1String& path, QByteArray *buffer, qint64 size, qint64 offset, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file read" << path << size << offset;
#endif
    return true;
}

bool RemoteFileOps::write(const QLatin1String& path, const QByteArray &data, qint64 offset)
{
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file write" << path << data << offset;
#endif
    return true;
}

bool RemoteFileOps::flush(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file flush" << path;
#endif
    return true;
}

bool RemoteFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file release" << path;
#endif
    return true;
}

bool RemoteFileOps::fsync(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file fsync" << path;
#endif
    return true;
}

bool RemoteFileOps::opendir(const QLatin1String& path, quint64* fh)
{
    Q_UNUSED(path);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file opendir" << path;
#endif
    return true;
}

bool RemoteFileOps::readdir(const QLatin1String& path, FileInfo* info, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(info);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file readdir" << path;
#endif
    return true;
}

bool RemoteFileOps::releasedir(const QLatin1String& path, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(fh);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file releasedir" << path;
#endif
    return true;
}

bool RemoteFileOps::fsyncdir(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file fsyncdir" << path;
#endif
    return true;
}

bool RemoteFileOps::utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod)
{
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file utime" << path << acc << mod;
#endif
    return true;
}
