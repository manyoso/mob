#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QThread>

class FileSystemPrivate;
class FileOps;
class FileSystem : public QThread {
    Q_OBJECT
public:
    FileSystem(FileOps* fileOps);
    ~FileSystem();

    static const QLatin1String& mountPoint();
    static const QLatin1String& root();
    static void setRoot(const QLatin1String& root);
    //! \brief Returns the root + path and path might or might not be copied.
    static QByteArray absolutePath(const char* path);

protected:
    virtual void run();

private:
    void stop();
    FileSystemPrivate* d;
};

#endif // filesystem_h
