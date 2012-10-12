#include "localfileops.h"

#define DEBUG_LOCALFILEOPS 0

bool LocalFileOps::readlink(const QLatin1String& path, QTextStream& buffer)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readlink" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    return false;
}

bool LocalFileOps::mkdir(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file mkdir" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return false;
}

bool LocalFileOps::rmdir(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rmdir" << path;
#endif
    Q_UNUSED(path);
    return false;
}

bool LocalFileOps::symlink(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file symlink" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return false;
}

bool LocalFileOps::rename(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rename" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return false;
}

bool LocalFileOps::link(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file link" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return false;
}

bool LocalFileOps::chmod(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chmod" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return false;
}

bool LocalFileOps::chown(const QLatin1String& path, quint32 uid, quint32 gid)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chown" << path << uid << gid;
#endif
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
    return false;
}

bool LocalFileOps::truncate(const QLatin1String& path, qint64 size)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file truncate" << path << size;
#endif
    Q_UNUSED(path);
    Q_UNUSED(size);
    return false;
}

bool LocalFileOps::write(const QLatin1String& path, const QByteArray &data, qint64 offset)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file write" << path << data << offset;
#endif
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
    return false;
}

bool LocalFileOps::flush(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file flush" << path;
#endif
    Q_UNUSED(path);
    return false;
}

bool LocalFileOps::fsync(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsync" << path;
#endif
    Q_UNUSED(path);
    return false;
}

bool LocalFileOps::fsyncdir(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsyncdir" << path;
#endif
    Q_UNUSED(path);
    return false;
}

bool LocalFileOps::utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file utime" << path << acc << mod;
#endif
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
    return false;
}
