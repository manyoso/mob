#ifndef fileops_h
#define fileops_h

#include <QtCore>

class Node;

/**
 * A file operations interface.
 */

class FileOps {
public:
    FileOps(Node* node) : m_node(node) {}

    Node* node() const { return m_node; }

    virtual bool getattr(const QString& path, QFileInfo*) const = 0;
    virtual bool readlink(const QString& path, QTextStream& buffer) const = 0;
    virtual bool create(const QString& path, QFile::Permissions) const = 0;
    virtual bool mkdir(const QString& path, QFile::Permissions) const = 0;
    virtual bool unlink(const QString& path) const = 0;
    virtual bool rmdir(const QString& path) const = 0;
    virtual bool symlink(const QString& path1, const QString& path2) const = 0;
    virtual bool rename(const QString& path1, const QString& path2) const = 0;
    virtual bool link(const QString& path1, const QString& path2) const = 0;
    virtual bool chmod(const QString& path, QFile::Permissions) const = 0;
    virtual bool chown(const QString& path, quint32 uid, quint32 gid) const = 0;
    virtual bool truncate(const QString& path, qint64 size) const = 0;
    virtual bool open(const QString& path) const = 0;
    virtual bool read(const QString& path, QByteArray *buffer, size_t size, qint64 offset) const = 0;
    virtual bool write(const QString& path, const QByteArray &data, qint64 offset) const = 0;
    virtual bool flush(const QString& path) const = 0;
    virtual bool release(const QString& path) const = 0;
    virtual bool fsync(const QString& path) const = 0;
    virtual bool opendir(const QString& path) const = 0;
    virtual bool readdir(const QString& path, QDir*) const = 0;
    virtual bool releasedir(const QString& path) const = 0;
    virtual bool fsyncdir(const QString& path) const = 0;
    virtual bool utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const = 0;

private:
    Node* m_node;
};

#endif // fileops_h
