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
    int rc = ::lstat(path.latin1(), &stbuf);
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

static mode_t permissionsToMode(QFile::Permissions p)
{
    mode_t m = 0;
    if (p & QFile::ReadOwner)   m |= S_IRUSR;
    if (p & QFile::WriteOwner)  m |= S_IWUSR;
    if (p & QFile::ExeOwner)    m |= S_IXUSR;
    if (p & QFile::ReadUser)    m |= S_IRUSR;
    if (p & QFile::WriteUser)   m |= S_IWUSR;
    if (p & QFile::ExeUser)     m |= S_IXUSR;
    if (p & QFile::ReadGroup)   m |= S_IRGRP;
    if (p & QFile::WriteGroup)  m |= S_IWGRP;
    if (p & QFile::ExeGroup)    m |= S_IXGRP;
    if (p & QFile::ReadOther)   m |= S_IROTH;
    if (p & QFile::WriteOther)  m |= S_IWOTH;
    if (p & QFile::ExeOther)    m |= S_IXOTH;
    return m;
}

bool LocalFileOps::create(const QLatin1String& path, QFile::Permissions perm, qint32 flags, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file create" << path << perm;
#endif

    int rc = ::open(path.latin1(), O_CREAT | flags, permissionsToMode(perm));
    if (rc == -1)
        m_error = errno;
    else if (fh)
        *fh = rc;
    return rc > 0;
}

bool LocalFileOps::unlink(const QLatin1String& path)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file unlink" << path;
#endif

    int rc = ::unlink(path.latin1());
    if (rc != 0)
        m_error = errno;
    return rc == 0;
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

bool LocalFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file release" << path << flags << fh;
#else
    Q_UNUSED(path);
    Q_UNUSED(flags);
#endif

    int rc = ::close(fh);
    if (rc != 0)
        m_error = errno;
    return rc == 0;
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
#if _DIRENT_HAVE_D_NAMLEN
        info->setName(QString::fromAscii(dir->d_name, dir->d_namlen));
#else
        info->setName(QString::fromAscii(dir->d_name));
#endif
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
