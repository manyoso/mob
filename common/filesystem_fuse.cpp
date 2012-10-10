#include "filesystem.h"

#include "node.h"
#include "remotefileops.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define FUSE_USE_VERSION 26
#include <errno.h>
#include <fuse.h>
#include <fuse/fuse_lowlevel.h>
#include <signal.h>

#define DEBUG_FILESYSTEM 0

QDebug operator<<(QDebug dbg, struct fuse_file_info* fi)
{
    if (!fi)
        dbg.nospace() << "(!fuse_file_info)";
    else
        dbg.nospace() << "(flags=" << fi->flags << ", fh=" << fi->fh << ")";
    return dbg.space();
}

struct FileSystemPrivate
{
    FileSystemPrivate(FileOps* ops)
    {
        m_fileOps = ops;
    }

    FileOps* m_fileOps;
};

static FileOps* fileOps()
{
    fuse_context* context = fuse_get_context();
    Q_ASSERT(context);
    if (!context)
        return 0;

    return static_cast<FileSystemPrivate*>(context->private_data)->m_fileOps;
}

static int fs_getattr(const char* path, struct stat* stbuf)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_getattr for" << path;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    FileInfo info;
    if (!ops->getattr(QLatin1String(path), &info))
        return ops->error();

    stbuf->st_ino       = info.serialNumber();
    stbuf->st_mode      = info.mode();
    stbuf->st_nlink     = info.numberOfHardLinks();
    stbuf->st_uid       = info.userId();
    stbuf->st_gid       = info.groupId();
    stbuf->st_rdev      = info.deviceId();
    stbuf->st_size      = info.size();
    stbuf->st_atime     = info.lastAccess().toTime_t();
    stbuf->st_mtime     = info.lastDataModification().toTime_t();
    stbuf->st_ctime     = info.lastStatusChange().toTime_t();
    stbuf->st_blocks    = info.numberOfBlocks();
    return 0;
}

static int fs_readlink(const char* path, char*, size_t)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_readlink" << path;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    return 0;
}

static int fs_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_create" << path << mode << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(mode);
    Q_UNUSED(fi);
    return 0;
}

static int fs_mkdir(const char* path, mode_t mode)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_mkdir" << path << mode;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(mode);
    return 0;
}

static int fs_unlink(const char* path)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_unlink" << path;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    return 0;
}

static int fs_rmdir(const char* path)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_rmdir" << path;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    return 0;
}

static int fs_symlink(const char* path1, const char* path2)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_symlink" << path1 << path2;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return 0;
}

static int fs_rename(const char* path1, const char* path2)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_rename" << path1 << path2;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return 0;
}

static int fs_link(const char* path1, const char* path2)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_link" << path1 << path2;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path1);
    Q_UNUSED(path2);
    return 0;
}

static int fs_chmod(const char* path, mode_t mode)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_chmod" << path << mode;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(mode);
    return 0;
}

static int fs_chown(const char* path, uid_t uid, gid_t gid)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_chown" << path << uid << gid;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(uid);
    Q_UNUSED(gid);
    return 0;
}

static int fs_truncate(const char* path, off_t offset)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_truncate" << path << offset;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(offset);
    return 0;
}

static int fs_open(const char* path, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_open" << path << fi;
#endif

    Q_ASSERT(fi);
    if (!fi)
        return -EAGAIN;

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    if (!ops->open(QLatin1String(path), fi->flags, &fi->fh))
        return ops->error();
    return 0;
}

static int fs_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_read" << path << size << offset << fi;
#endif

    Q_UNUSED(fi);
    if (!fi)
        return -EAGAIN;

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    QByteArray bytes = QByteArray::fromRawData(buf, size);
    if (!ops->read(QLatin1String(path), &bytes, size, offset, fi->fh))
        return ops->error();
    return 0;
}

static int fs_write(const char* path, const char* data, size_t size, off_t offset, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_write" << path << data << size << offset << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(data);
    Q_UNUSED(size);
    Q_UNUSED(offset);
    Q_UNUSED(fi);
    return 0;
}

static int fs_flush(const char* path, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_flush" << path << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(fi);
    return 0;
}

static int fs_release(const char* path, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_release" << path << fi;
#endif

    Q_ASSERT(fi);
    if (!fi)
        return -EAGAIN;

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    if (!ops->release(QLatin1String(path), fi->flags, fi->fh))
        return ops->error();
    return 0;
}

static int fs_fsync(const char* path, int, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_fsync" << path << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(fi);
    return 0;
}

static int fs_opendir(const char* path, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_opendir" << path << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(fi);
    return 0;
}

static int fs_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_readdir" << path << filler << offset << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(buf);
    Q_UNUSED(filler);
    Q_UNUSED(offset);
    Q_UNUSED(fi);
    return 0;
}

static int fs_releasedir(const char* path, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_releasedir" << path << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(fi);
    return 0;
}

static int fs_fsyncdir(const char* path, int, struct fuse_file_info* fi)
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_fsyncdir" << path << fi;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(fi);
    return 0;
}

static int fs_utimens(const char* path, const struct timespec tv[2])
{
#if DEBUG_FILESYSTEM
    qDebug() << "fs_utimenss" << path << tv;
#endif

    FileOps* ops = fileOps();
    if (!ops)
        return -EAGAIN;

    Q_UNUSED(path);
    Q_UNUSED(tv);
    return 0;
}

static struct fuse* s_fuse = 0;
static void* fs_init(struct fuse_conn_info* conn)
{
    Q_UNUSED(conn);
#if DEBUG_FILESYSTEM
    qDebug() << "fs_init";
#endif
    fuse_context* context = fuse_get_context();
    s_fuse = context->fuse;
    return context->private_data;
}

static void fs_destroy(void* p)
{
    Q_UNUSED(p);
#if DEBUG_FILESYSTEM
    qDebug() << "fs_destroy";
#endif
    s_fuse = 0;
}

FileSystem::FileSystem(FileOps* ops)
    : QThread(0)
    , d(new FileSystemPrivate(ops))
{
    // The signal handlers *should not* be called by the main thread
    sigset_t x;
    sigemptyset (&x);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGINT);
    sigaddset(&x, SIGTERM);
    sigaddset(&x, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &x, NULL);
}

FileSystem::~FileSystem()
{
    stop();
    delete d;
}

const QString& FileSystem::mountPoint()
{
    static QString mount = QDir::tempPath().append(QCoreApplication::applicationName());
    return mount;
}

void FileSystem::stop()
{
    if (!s_fuse && wait(1000)) {
        terminate();
        return;
    }

#if DEBUG_FILESYSTEM
    qDebug() << "Stopping fuse filesystem at" << mountPoint().toAscii().constData();
#endif

    Q_ASSERT(s_fuse);
    fuse_exit(s_fuse);
    errno = 0;
    struct stat stbuf;
    QString foo = mountPoint();
    foo.append("/foo");
    stat(foo.toAscii().constData(), &stbuf);
    if (!wait(5000)) {
        terminate();
        wait();
    }
}

void FileSystem::run()
{
    // The signal handlers *should* be called by the filesystem thread
    sigset_t x;
    sigemptyset (&x);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGINT);
    sigaddset(&x, SIGTERM);
    sigaddset(&x, SIGPIPE);
    pthread_sigmask(SIG_UNBLOCK, &x, NULL);

    QString mount = mountPoint();

#if DEBUG_FILESYSTEM
    qDebug() << "Starting fuse filesystem at" << mount;
#endif

    // First delete the directory if it exists since we want to control the mode explicitly
    if (QFile::exists(mount) && rmdir(mount.toAscii().constData()) != 0) {
        qDebug() << "ERROR: Could not create mount point for fileystem at" << mount << "because it already exists!";
        return;
    }

    // Make the mount point only RWX for the owner of the process and no one else
    if (QFile::exists(mount) || mkdir(mount.toAscii().constData(), S_IRUSR | S_IWUSR | S_IXUSR) != 0) {
        qDebug() << "ERROR: Could not create mount point for fileystem at" << mount << "!";
        return;
    }

    char  arg0[] = "mobd";
    char  arg2[] = "-s";
    char  arg3[] = "-f";
    char* argv[] = { &arg0[0], mount.toAscii().data(), &arg2[0], &arg3[0], NULL };
    int argc = 4;

    fuse_operations operations;
    memset(&operations, 0, sizeof(fuse_operations));

    operations.getattr      = fs_getattr;
    operations.readlink     = fs_readlink;
    operations.create       = fs_create;
    operations.mkdir        = fs_mkdir;
    operations.unlink       = fs_unlink;
    operations.rmdir        = fs_rmdir;
    operations.symlink      = fs_symlink;
    operations.rename       = fs_rename;
    operations.link         = fs_link;
    operations.chmod        = fs_chmod;
    operations.chown        = fs_chown;
    operations.truncate     = fs_truncate;
    operations.open         = fs_open;
    operations.read         = fs_read;
    operations.write        = fs_write;
    operations.flush        = fs_flush;
    operations.release      = fs_release;
    operations.fsync        = fs_fsync;
    operations.opendir      = fs_opendir;
    operations.readdir      = fs_readdir;
    operations.releasedir   = fs_releasedir;
    operations.fsyncdir     = fs_fsyncdir;
    operations.utimens      = fs_utimens;
    operations.init         = fs_init;
    operations.destroy      = fs_destroy;

    fuse_main(argc, argv, &operations, static_cast<void*>(d));

    // Clean up if possible
    rmdir(mount.toAscii().constData());
}
