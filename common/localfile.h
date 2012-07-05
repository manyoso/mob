#ifndef localfile_h
#define localfile_h

#include "fileoperations.h"

class LocalFile : public FileOperations {
public:
    LocalFile(Node* node) : FileOperations(node) {}

    bool getattr(const QString& path, QFileInfo*) const;
    bool readlink(const QString& path, QTextStream& buffer) const;
    bool create(const QString& path, QFile::Permissions) const;
    bool mkdir(const QString& path, QFile::Permissions) const;
    bool unlink(const QString& path) const;
    bool rmdir(const QString& path) const;
    bool symlink(const QString& path1, const QString& path2) const;
    bool rename(const QString& path1, const QString& path2) const;
    bool link(const QString& path1, const QString& path2) const;
    bool chmod(const QString& path, QFile::Permissions) const;
    bool chown(const QString& path, quint32 uid, quint32 gid) const;
    bool truncate(const QString& path, qint64 size) const;
    bool open(const QString& path) const;
    bool read(const QString& path, QByteArray *buffer, size_t size, qint64 offset) const;
    bool write(const QString& path, const QByteArray &data, qint64 offset) const;
    bool flush(const QString& path) const;
    bool release(const QString& path) const;
    bool fsync(const QString& path) const;
    bool opendir(const QString& path) const;
    bool readdir(const QString& path, QDir*) const;
    bool releasedir(const QString& path) const;
    bool fsyncdir(const QString& path) const;
    bool utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const;
};

#endif // localfile_h
