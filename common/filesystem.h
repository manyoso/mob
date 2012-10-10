#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QThread>

class FileSystemPrivate;
class RemoteFileOps;
class FileSystem : public QThread {
    Q_OBJECT
public:
    FileSystem(RemoteFileOps* fileOps);
    ~FileSystem();

    static const QString& mountPoint();

protected:
    virtual void run();

private:
    void stop();
    FileSystemPrivate* d;
};

#endif // filesystem_h
