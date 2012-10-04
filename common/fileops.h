#ifndef fileops_h
#define fileops_h

#include <QtCore>

class Node;

/**
 * A file operations interface.
 */

struct FileInfo {
    FileInfo()
        : serialNumber(-1)
        , mode(-1)
        , numberOfHardLinks(0)
        , userId(-1)
        , groupId(-1)
        , deviceId(-1)
        , size(0)
        , numberOfBlocks(0)
    {}

    quint64     serialNumber;
    quint16     mode;
    quint16     numberOfHardLinks;
    quint32     userId;
    quint32     groupId;
    qint32      deviceId;
    qint64      size;
    QDateTime   lastAccess;
    QDateTime   lastDataModification;
    QDateTime   lastStatusChange;
    qint64      numberOfBlocks;
};

class FileOps {
public:
    FileOps(Node* node) : m_node(node) {}

    Node* node() const { return m_node; }

    virtual bool getattr(const QString& path, FileInfo*) const = 0;
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
    virtual bool readdir(const QString& path, FileInfo*) const = 0;
    virtual bool releasedir(const QString& path) const = 0;
    virtual bool fsyncdir(const QString& path) const = 0;
    virtual bool utime(const QString& path, const QDateTime& acc, const QDateTime& mod) const = 0;

    quint8 error() const { return m_error; }

private:
    Node* m_node;
    quint8 m_error;
};

#endif // fileops_h
