#ifndef fileops_h
#define fileops_h

#include <QtCore>
#include <QtNetwork/QHostAddress>

#include "message.h"

/**
 * A file operations interface.
 */

class FileInfo : public Message {
    Q_OBJECT
    Q_PROPERTY(quint64 serialNumber READ serialNumber WRITE setSerialNumber STORED true)
    Q_PROPERTY(quint16 mode READ mode WRITE setMode STORED true)
    Q_PROPERTY(quint16 numberOfHardLinks READ numberOfHardLinks WRITE setNumberOfHardLinks STORED true)
    Q_PROPERTY(quint32 userId READ userId WRITE setUserId STORED true)
    Q_PROPERTY(quint32 groupId READ groupId WRITE setGroupId STORED true)
    Q_PROPERTY(qint32 deviceId READ deviceId WRITE setDeviceId STORED true)
    Q_PROPERTY(qint64 size READ size WRITE setSize STORED true)
    Q_PROPERTY(QDateTime lastAccess READ lastAccess WRITE setLastAccess STORED true)
    Q_PROPERTY(QDateTime lastDataModification READ lastDataModification WRITE setLastDataModification STORED true)
    Q_PROPERTY(QDateTime lastStatusChange READ lastStatusChange WRITE setLastStatusChange STORED true)
    Q_PROPERTY(qint64 numberOfBlocks READ numberOfBlocks WRITE setNumberOfBlocks STORED true)
public:
    FileInfo()
        : Message(Message::FileInfo)
        , m_serialNumber(0)
        , m_mode(0)
        , m_numberOfHardLinks(0)
        , m_userId(0)
        , m_groupId(0)
        , m_deviceId(0)
        , m_size(0)
        , m_numberOfBlocks(0)
    {}

    quint64 serialNumber() const { return m_serialNumber; }
    void setSerialNumber(quint64 serialNumber) { m_serialNumber = serialNumber; }

    quint16 mode() const { return m_mode; }
    void setMode(quint16 mode) { m_mode = mode; }

    quint16 numberOfHardLinks() const { return m_numberOfHardLinks; }
    void setNumberOfHardLinks(quint16 numberOfHardLinks) { m_numberOfHardLinks = numberOfHardLinks; }

    quint32 userId() const { return m_userId; }
    void setUserId(quint64 userId) { m_userId = userId; }

    quint32 groupId() const { return m_groupId; }
    void setGroupId(quint32 groupId) { m_groupId = groupId; }

    qint32 deviceId() const { return m_deviceId; }
    void setDeviceId(quint32 deviceId) { deviceId = m_deviceId; }

    qint64 size() const { return m_size; }
    void setSize(quint64 size) { m_size = size; }

    QDateTime lastAccess() const { return m_lastAccess; }
    void setLastAccess(const QDateTime& lastAccess) { m_lastAccess = lastAccess; }

    QDateTime lastDataModification() const { return m_lastDataModification; }
    void setLastDataModification(const QDateTime& lastDataModification) { m_lastDataModification = lastDataModification; }

    QDateTime lastStatusChange() const { return m_lastStatusChange; }
    void setLastStatusChange(const QDateTime& lastStatusChange) { m_lastStatusChange = lastStatusChange; }

    qint64 numberOfBlocks() const { return m_numberOfBlocks; }
    void setNumberOfBlocks(quint64 numberOfBlocks) { m_numberOfBlocks = numberOfBlocks; }

    static Message* createMessage() { return new FileInfo; }

private:
    quint64     m_serialNumber;
    quint16     m_mode;
    quint16     m_numberOfHardLinks;
    quint32     m_userId;
    quint32     m_groupId;
    qint32      m_deviceId;
    qint64      m_size;
    QDateTime   m_lastAccess;
    QDateTime   m_lastDataModification;
    QDateTime   m_lastStatusChange;
    qint64      m_numberOfBlocks;
};

INSTALL_MESSAGE_FACTORY(FileInfo, &FileInfo::createMessage);

class FileOps {
public:
    FileOps(const QHostAddress& address) : m_address(address) {}
    virtual ~FileOps() {}

    QHostAddress address() const { return m_address; }

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
    QHostAddress m_address;
    quint8 m_error;
};

#endif // fileops_h
