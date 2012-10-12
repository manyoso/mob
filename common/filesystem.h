#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QThread>

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

protected:
    virtual void run();

private:
    void stop();
    FileOps* m_fileOps;
    QByteArray m_root;
    QLatin1String m_rootString;
};

#endif // filesystem_h
