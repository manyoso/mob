#include "localfileops.h"

#define DEBUG_LOCALFILEOPSOPS 0

bool LocalFileOps::getattr(const QString& path, QFileInfo* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file getattr" << path;
#endif
    return true;
}

bool LocalFileOps::readlink(const QString& path, QTextStream& buffer) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readlink" << path;
#endif
    return true;
}

bool LocalFileOps::create(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file create" << path << perm;
#endif
    return true;
}

bool LocalFileOps::mkdir(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file mkdir" << path << perm;
#endif
    return true;
}

bool LocalFileOps::unlink(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file unlink" << path;
#endif
    return true;
}

bool LocalFileOps::rmdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rmdir" << path;
#endif
    return true;
}

bool LocalFileOps::symlink(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file symlink" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::rename(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rename" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::link(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file link" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::chmod(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chmod" << path << perm;
#endif
    return true;
}

bool LocalFileOps::chown(const QString& path, quint32 uid, quint32 gid) const
{
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chown" << path << uid << gid;
#endif
    return true;
}

bool LocalFileOps::truncate(const QString& path, qint64 size) const
{
    Q_UNUSED(path);
    Q_UNUSED(size);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file truncate" << path << size;
#endif
    return true;
}

bool LocalFileOps::open(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file open" << path;
#endif
    return true;
}

bool LocalFileOps::read(const QString& path, QByteArray *buffer, size_t size, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file read" << path << size << offset;
#endif
    return true;
}

bool LocalFileOps::write(const QString& path, const QByteArray &data, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file write" << path << data << offset;
#endif
    return true;
}

bool LocalFileOps::flush(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file flush" << path;
#endif
    return true;
}

bool LocalFileOps::release(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file release" << path;
#endif
    return true;
}

bool LocalFileOps::fsync(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsync" << path;
#endif
    return true;
}

bool LocalFileOps::opendir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file opendir" << path;
#endif
    return true;
}

bool LocalFileOps::readdir(const QString& path, QDir* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readdir" << path;
#endif
    return true;
}

bool LocalFileOps::releasedir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file releasedir" << path;
#endif
    return true;
}

bool LocalFileOps::fsyncdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsyncdir" << path;
#endif
    return true;
}

bool LocalFileOps::utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const
{
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file utime" << path << acc << mod;
#endif
    return true;
}
