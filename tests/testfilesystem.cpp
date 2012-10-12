#include "testfilesystem.h"

#include "filesystem.h"
#include "localfileops.h"
#include "remotefileops.h"

void TestFileSystem::testLocalFileOps()
{
    LocalFileOps localFileOps(QHostAddress::LocalHost);
    FileSystem fileSystem(&localFileOps);
    fileSystem.start();

    // Test reading root directory

    // Test file creation

    // Test directory creation

    // Test file info

    // Test reading file

    // Test file deletion

    // Test directory deletion
}
