#include "remotefileops.h"

#define DEBUG_REMOTEFILEOPS 0

bool RemoteFileOps::getattr(const QLatin1String& path, FileInfo* info)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file getattr" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(info);
    return true;
}

bool RemoteFileOps::readlink(const QLatin1String& path, QTextStream& buffer)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file readlink" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    return true;
}

bool RemoteFileOps::create(const QLatin1String& path, QFile::Permissions perm, qint32 flags, quint64* fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file create" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::mkdir(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file mkdir" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return true;
}

bool RemoteFileOps::unlink(const QLatin1String& path)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file unlink" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool RemoteFileOps::rmdir(const QLatin1String& path)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file rmdir" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool RemoteFileOps::symlink(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file symlink" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool RemoteFileOps::rename(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file rename" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool RemoteFileOps::link(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file link" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool RemoteFileOps::chmod(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file chmod" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return true;
}

bool RemoteFileOps::chown(const QLatin1String& path, quint32 uid, quint32 gid)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file chown" << path << uid << gid;
#endif
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
    return true;
}

bool RemoteFileOps::truncate(const QLatin1String& path, qint64 size)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file truncate" << path << size;
#endif
    Q_UNUSED(path);
    Q_UNUSED(size);
    return true;
}

bool RemoteFileOps::open(const QLatin1String& path, qint32 flags, quint64* fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file open" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::read(const QLatin1String& path, QByteArray *buffer, qint64 size, qint64 offset, quint64 fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file read" << path << size << offset;
#endif
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::write(const QLatin1String& path, const QByteArray &data, qint64 offset)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file write" << path << data << offset;
#endif
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
    return true;
}

bool RemoteFileOps::flush(const QLatin1String& path)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file flush" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool RemoteFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file release" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::fsync(const QLatin1String& path)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file fsync" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool RemoteFileOps::opendir(const QLatin1String& path, quint64* fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file opendir" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::readdir(const QLatin1String& path, quint64 offset, FileInfo* info, quint64 fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file readdir" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(offset);
    Q_UNUSED(info);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::releasedir(const QLatin1String& path, quint64 fh)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file releasedir" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(fh);
    return true;
}

bool RemoteFileOps::fsyncdir(const QLatin1String& path)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file fsyncdir" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool RemoteFileOps::utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod)
{
#if DEBUG_REMOTEFILEOPS
    qDebug() << "Remote file utime" << path << acc << mod;
#endif
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
    return true;
}
