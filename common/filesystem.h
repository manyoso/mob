#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QByteArray>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

class FileSystemPrivate;
class FileOps;
class FileSystem : public QThread {
    Q_OBJECT
public:
    FileSystem(FileOps* fileOps);
    ~FileSystem();

    //! \brief Returns the file operations object for this file system.
    FileOps* fileOps() const { return m_fileOps; }

    //! \brief Returns the temporary mount point.
    const QLatin1String& mountPoint() const;

    //! \brief Returns the mirrored root.
    const QLatin1String& root() const;

    //! \brief Sets the mirrord root.
    void setRoot(const QLatin1String& root);

    //! \brief Returns the root + path where path might or might not be deep copied.
    QByteArray absolutePath(const char* path) const;

    //! \brief Starts the filesystem and blocks the calling thread until it is running and mounted.
    void startFileSystem();

protected:
    virtual void run();

private:
    void stop();
    FileOps* m_fileOps;
    QByteArray m_root;
    QLatin1String m_rootString;
    bool m_started;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    friend class FileSystemPrivate;
    FileSystemPrivate* d;
};

#endif // filesystem_h
