#include "localfile.h"

#define DEBUG_LOCALFILE 0

bool LocalFile::getattr(const QString& path, QFileInfo* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILE
    qDebug() << "Local file getattr" << path;
#endif
    return true;
}

bool LocalFile::readlink(const QString& path, QTextStream& buffer) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
#if DEBUG_LOCALFILE
    qDebug() << "Local file readlink" << path;
#endif
    return true;
}

bool LocalFile::create(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Local file create" << path << perm;
#endif
    return true;
}

bool LocalFile::mkdir(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Local file mkdir" << path << perm;
#endif
    return true;
}

bool LocalFile::unlink(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file unlink" << path;
#endif
    return true;
}

bool LocalFile::rmdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file rmdir" << path;
#endif
    return true;
}

bool LocalFile::symlink(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Local file symlink" << path1 << path2;
#endif
    return true;
}

bool LocalFile::rename(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Local file rename" << path1 << path2;
#endif
    return true;
}

bool LocalFile::link(const QString& path1, const QString& path2) const
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILE
    qDebug() << "Local file link" << path1 << path2;
#endif
    return true;
}

bool LocalFile::chmod(const QString& path, QFile::Permissions perm) const
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILE
    qDebug() << "Local file chmod" << path << perm;
#endif
    return true;
}

bool LocalFile::chown(const QString& path, quint32 uid, quint32 gid) const
{
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
#if DEBUG_LOCALFILE
    qDebug() << "Local file chown" << path << uid << gid;
#endif
    return true;
}

bool LocalFile::truncate(const QString& path, qint64 size) const
{
    Q_UNUSED(path);
    Q_UNUSED(size);
#if DEBUG_LOCALFILE
    qDebug() << "Local file truncate" << path << size;
#endif
    return true;
}

bool LocalFile::open(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file open" << path;
#endif
    return true;
}

bool LocalFile::read(const QString& path, QByteArray *buffer, size_t size, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILE
    qDebug() << "Local file read" << path << size << offset;
#endif
    return true;
}

bool LocalFile::write(const QString& path, const QByteArray &data, qint64 offset) const
{
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILE
    qDebug() << "Local file write" << path << data << offset;
#endif
    return true;
}

bool LocalFile::flush(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file flush" << path;
#endif
    return true;
}

bool LocalFile::release(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file release" << path;
#endif
    return true;
}

bool LocalFile::fsync(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file fsync" << path;
#endif
    return true;
}

bool LocalFile::opendir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file opendir" << path;
#endif
    return true;
}

bool LocalFile::readdir(const QString& path, QDir* info) const
{
    Q_UNUSED(path);
    Q_UNUSED(info);
#if DEBUG_LOCALFILE
    qDebug() << "Local file readdir" << path;
#endif
    return true;
}

bool LocalFile::releasedir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file releasedir" << path;
#endif
    return true;
}

bool LocalFile::fsyncdir(const QString& path) const
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILE
    qDebug() << "Local file fsyncdir" << path;
#endif
    return true;
}

bool LocalFile::utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const
{
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
#if DEBUG_LOCALFILE
    qDebug() << "Local file utime" << path << acc << mod;
#endif
    return true;
}
