#ifndef localfileops_h
#define localfileops_h

#include "fileops.h"

class LocalFileOps : public FileOps {
public:
    LocalFileOps(const QHostAddress& address) : FileOps(address) {}

    bool getattr(const QLatin1String& path, FileInfo*);
    bool readlink(const QLatin1String& path, QTextStream& buffer);
    bool create(const QLatin1String& path, QFile::Permissions);
    bool mkdir(const QLatin1String& path, QFile::Permissions);
    bool unlink(const QLatin1String& path);
    bool rmdir(const QLatin1String& path);
    bool symlink(const QLatin1String& path1, const QLatin1String& path2);
    bool rename(const QLatin1String& path1, const QLatin1String& path2);
    bool link(const QLatin1String& path1, const QLatin1String& path2);
    bool chmod(const QLatin1String& path, QFile::Permissions);
    bool chown(const QLatin1String& path, quint32 uid, quint32 gid);
    bool truncate(const QLatin1String& path, qint64 size);
    bool open(const QLatin1String& path, qint32 flags, quint64* fh);
    bool read(const QLatin1String& path, QByteArray *buffer, qint64 size, qint64 offset, quint64 fh);
    bool write(const QLatin1String& path, const QByteArray &data, qint64 offset);
    bool flush(const QLatin1String& path);
    bool release(const QLatin1String& path, qint32 flags, quint64 fh);
    bool fsync(const QLatin1String& path);
    bool opendir(const QLatin1String& path, quint64* fh);
    bool readdir(const QLatin1String& path, quint64 offset, FileInfo*, quint64 fh);
    bool releasedir(const QLatin1String& path, quint64 fh);
    bool fsyncdir(const QLatin1String& path);
    bool utime(const QLatin1String& path, const QDateTime& acc, const QDateTime& mod);
};

#endif // localfileops_h
