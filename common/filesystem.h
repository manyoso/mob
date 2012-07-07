#ifndef filesystem_h
#define filesystem_h

#include <QtCore/QThread>

class FileSystemPrivate;
class Node;
class FileSystem : public QThread {
    Q_OBJECT
public:
    FileSystem(Node* node);
    ~FileSystem();

    Node* node() const;
    QString mountPoint() const;

protected:
    virtual void run();

private:
    FileSystemPrivate* d;
};

#endif // filesystem_h
