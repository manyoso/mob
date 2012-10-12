#include "testfilesystem.h"

#include "filesystem.h"
#include "localfileops.h"
#include "remotefileops.h"

void TestFileSystem::testLocalFileOps()
{
    QDir root(QDir::tempPath());
    QVERIFY(root.exists());
    if (!root.exists("testLocalFileOps")) {
        QVERIFY(root.mkdir("testLocalFileOps"));
    }
    QVERIFY(root.cd("testLocalFileOps"));
    QByteArray canonicalPath = root.canonicalPath().toLatin1();

    LocalFileOps localFileOps(QHostAddress::LocalHost);
    FileSystem fileSystem(&localFileOps);
    fileSystem.setRoot(QLatin1String(canonicalPath.constData()));
    fileSystem.startFileSystem();

    // Test reading mounted directory
    QDir mount(fileSystem.mountPoint());
    QVERIFY(mount.exists());
    QVERIFY(mount.isAbsolute());
    QVERIFY(mount.isReadable());
    QVERIFY(!mount.isRelative());
    QCOMPARE(mount.count(), uint(2));

    QFileInfoList files = mount.entryInfoList();
    QCOMPARE(files.count(), 2);

    // The '.' entry
    QFileInfo fileInfo = files.at(0);
    QVERIFY(fileInfo.exists());
    QCOMPARE(fileInfo.fileName(), QLatin1String("."));

    // The '..' entry
    fileInfo = files.at(1);
    QVERIFY(fileInfo.exists());
    QCOMPARE(fileInfo.fileName(), QLatin1String(".."));

    // Test file creation and deletion
    QFile file(mount.canonicalPath().append(QDir::separator()).append("foo"));
    QVERIFY(!file.exists());
    QVERIFY(file.open(QIODevice::WriteOnly));
    QVERIFY(file.exists());
    file.close();
    file.remove();

    // Test directory creation

    // Test file info

    // Test reading file

    // Test file deletion

    // Test directory deletion

    QVERIFY(root.cdUp());
    QVERIFY(root.rmdir("testLocalFileOps"));
}
