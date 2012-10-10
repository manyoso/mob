#include "testfilesystem.h"

#include "filesystem.h"
#include "localfileops.h"
#include "remotefileops.h"

void TestFileSystem::test()
{
    LocalFileOps localFileOps(QHostAddress::LocalHost);
    FileSystem fileSystem(&localFileOps);
    fileSystem.start();
}
