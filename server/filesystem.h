#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QThread>

class FileSystemPrivate;
class FileSystem : public QThread {
    Q_OBJECT
public:
    FileSystem(QObject* parent);
    ~FileSystem();

    QString mountPoint() const;

protected:
    virtual void run();

private:
    FileSystemPrivate* d;
};

#endif // filesystem_h
