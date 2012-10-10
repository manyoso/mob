#include "localfileops.h"

#include <dirent.h>
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
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readlink" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    return true;
}

bool LocalFileOps::create(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file create" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return true;
}

bool LocalFileOps::mkdir(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file mkdir" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return true;
}

bool LocalFileOps::unlink(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file unlink" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool LocalFileOps::rmdir(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rmdir" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool LocalFileOps::symlink(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file symlink" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool LocalFileOps::rename(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file rename" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool LocalFileOps::link(const QLatin1String& path1, const QLatin1String& path2)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file link" << path1 << path2;
#endif
    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return true;
}

bool LocalFileOps::chmod(const QLatin1String& path, QFile::Permissions perm)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chmod" << path << perm;
#endif
    Q_UNUSED(path);
    Q_UNUSED(perm);
    return true;
}

bool LocalFileOps::chown(const QLatin1String& path, quint32 uid, quint32 gid)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file chown" << path << uid << gid;
#endif
    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
    return true;
}

bool LocalFileOps::truncate(const QLatin1String& path, qint64 size)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file truncate" << path << size;
#endif
    Q_UNUSED(path);
    Q_UNUSED(size);
    return true;
}

bool LocalFileOps::open(const QLatin1String& path, qint32 flags, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file open" << path << flags << fh;
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
    qDebug() << "Local file read" << path << size << offset << fh;
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
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file write" << path << data << offset;
#endif
    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(offset);
    return true;
}

bool LocalFileOps::flush(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file flush" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool LocalFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file release" << path << flags << fh;
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
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsync" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool LocalFileOps::opendir(const QLatin1String& path, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file opendir" << path << fh;
#endif
    DIR* dir = ::opendir(path.latin1());
    if (!dir)
        m_error = errno;
    else if (fh)
        *fh = reinterpret_cast<quint64>(dir);
    return dir;
}

bool LocalFileOps::readdir(const QLatin1String& path, quint64 offset, FileInfo* info, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readdir" << path;
#else
    Q_UNUSED(path);
    Q_UNUSED(offset);
#endif

    struct dirent* dir = ::readdir(reinterpret_cast<DIR*>(fh));
    if (!dir)
        m_error = errno;
    else if (info) {
        info->setName(QString::fromAscii(dir->d_name, dir->d_namlen));
        info->setSerialNumber(dir->d_ino);
        info->setMode(dir->d_type << 12);
    }
    return dir;
}

bool LocalFileOps::releasedir(const QLatin1String& path, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file releasedir" << path << fh;
#else
    Q_UNUSED(path);
#endif
    int rc = ::closedir(reinterpret_cast<DIR*>(fh));
    if (rc != 0)
        m_error = errno;
    return rc == 0;
}

bool LocalFileOps::fsyncdir(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file fsyncdir" << path;
#endif
    Q_UNUSED(path);
    return true;
}

bool LocalFileOps::utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file utime" << path << acc << mod;
#endif
    Q_UNUSED(path);
    Q_UNUSED(acc);
    Q_UNUSED(mod);
    return true;
}
