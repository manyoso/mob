#include "localfileops.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEBUG_LOCALFILEOPS 0

bool LocalFileOps::getattr(const QLatin1String& path, FileInfo* info)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file getattr" << path;
#endif

    struct stat stbuf;
    int rc = stat(path.latin1(), &stbuf);
    if (rc != 0)
        m_error = errno;
    else if (info) {
        info->setSerialNumber(stbuf.st_ino);
        info->setMode(stbuf.st_mode);
        info->setNumberOfHardLinks(stbuf.st_nlink);
        info->setUserId(stbuf.st_uid);
        info->setGroupId(stbuf.st_gid);
        info->setDeviceId(stbuf.st_rdev);
        info->setSize(stbuf.st_size);
        info->setLastAccess(QDateTime::fromTime_t(stbuf.st_atime));
        info->setLastDataModification(QDateTime::fromTime_t(stbuf.st_mtime));
        info->setLastStatusChange(QDateTime::fromTime_t(stbuf.st_ctime));
        info->setNumberOfBlocks(stbuf.st_blocks);
    }

    return rc == 0;
}

bool LocalFileOps::readlink(const QLatin1String& path, QTextStream& buffer)
{
    Q_UNUSED(path);
    Q_UNUSED(buffer);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readlink" << path;
#endif
    return true;
}

bool LocalFileOps::create(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file create" << path << perm;
#endif
    return true;
}

bool LocalFileOps::mkdir(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file mkdir" << path << perm;
#endif
    return true;
}

bool LocalFileOps::unlink(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file unlink" << path;
#endif
    return true;
}

bool LocalFileOps::rmdir(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rmdir" << path;
#endif
    return true;
}

bool LocalFileOps::symlink(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file symlink" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::rename(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rename" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::link(const QLatin1String& path1, const QLatin1String& path2)
{
    Q_UNUSED(path1);
    Q_UNUSED(path2);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file link" << path1 << path2;
#endif
    return true;
}

bool LocalFileOps::chmod(const QLatin1String& path, QFile::Permissions perm)
{
    Q_UNUSED(path);
    Q_UNUSED(perm);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chmod" << path << perm;
#endif
    return true;
}

bool LocalFileOps::chown(const QLatin1String& path, quint32 uid, quint32 gid)
{
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chown" << path << uid << gid;
#endif
    return true;
}

bool LocalFileOps::truncate(const QLatin1String& path, qint64 size)
{
    Q_UNUSED(path);
    Q_UNUSED(size);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file truncate" << path << size;
#endif
    return true;
}

bool LocalFileOps::open(const QLatin1String& path, qint32 flags, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file open" << path << flags;
#endif

    int rc = ::open(path.latin1(), flags);
    if (rc == -1)
        m_error = errno;
    else if (fh)
        *fh = rc;
    return rc > 0;
}

bool LocalFileOps::read(const QLatin1String& path, QByteArray *buffer, qint64 size, qint64 offset, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file read" << path << size << offset;
#else
    Q_UNUSED(path);
#endif

    Q_ASSERT(buffer);
    int rc = ::pread(fh, buffer->data(), size, offset);
    if (rc == -1)
        m_error = errno;
    return rc > 0;
}

bool LocalFileOps::write(const QLatin1String& path, const QByteArray &data, qint64 offset)
{
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file write" << path << data << offset;
#endif
    return true;
}

bool LocalFileOps::flush(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file flush" << path;
#endif
    return true;
}

bool LocalFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file release" << path << flags;
#else
    Q_UNUSED(path);
    Q_UNUSED(flags);
#endif

    int rc = ::close(fh);
    if (rc == -1)
        m_error = errno;
    return rc == 0;
}

bool LocalFileOps::fsync(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsync" << path;
#endif
    return true;
}

bool LocalFileOps::opendir(const QLatin1String& path, quint64* fh)
{
    Q_UNUSED(path);
    Q_UNUSED(fh);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file opendir" << path;
#endif
    return true;
}

bool LocalFileOps::readdir(const QLatin1String& path, FileInfo* info, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(info);
    Q_UNUSED(fh);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readdir" << path;
#endif
    return true;
}

bool LocalFileOps::releasedir(const QLatin1String& path, quint64 fh)
{
    Q_UNUSED(path);
    Q_UNUSED(fh);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file releasedir" << path;
#endif
    return true;
}

bool LocalFileOps::fsyncdir(const QLatin1String& path)
{
    Q_UNUSED(path);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsyncdir" << path;
#endif
    return true;
}

bool LocalFileOps::utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod)
{
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file utime" << path << acc << mod;
#endif
    return true;
}
