#include "localfileops.h"

#define DEBUG_LOCALFILEOPS 0

bool LocalFileOps::getattr(const QLatin1String& path, FileInfo* info)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file getattr" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(info);
    return true;
}

bool LocalFileOps::open(const QLatin1String& path, qint32 flags, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file open" << path << flags << fh;
#endif
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
    return true;
}

bool LocalFileOps::read(const QLatin1String& path, QByteArray *buffer, qint64 size, qint64 offset, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file read" << path << size << offset << fh;
#endif
    Q_UNUSED(path);
    Q_UNUSED(buffer);
    Q_UNUSED(size);
    Q_UNUSED(offset);
    Q_UNUSED(fh);
    return true;
}

bool LocalFileOps::release(const QLatin1String& path, qint32 flags, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file release" << path << flags << fh;
#endif
    Q_UNUSED(path);
    Q_UNUSED(flags);
    Q_UNUSED(fh);
    return true;
}

bool LocalFileOps::opendir(const QLatin1String& path, quint64* fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file opendir" << path << fh;
#endif
    Q_UNUSED(path);
    Q_UNUSED(fh);
    return true;
}

bool LocalFileOps::readdir(const QLatin1String& path, quint64 offset, FileInfo* info, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file readdir" << path;
#endif
    Q_UNUSED(path);
    Q_UNUSED(offset);
    Q_UNUSED(info);
    Q_UNUSED(fh);
    return true;
}

bool LocalFileOps::releasedir(const QLatin1String& path, quint64 fh)
{
#if DEBUG_LOCALFILEOPS
    qDebug() << "Local file releasedir" << path << fh;
#endif
    Q_UNUSED(path);
    Q_UNUSED(fh);
    return true;
}
