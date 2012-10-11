INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/filesystem.cpp \
    $$PWD/global.cpp \
    $$PWD/localfileops.cpp \
    $$PWD/message.cpp \
    $$PWD/messagehandler.cpp \
    $$PWD/messageserver.cpp \
    $$PWD/node.cpp \
    $$PWD/remotefileops.cpp

HEADERS += \
    $$PWD/fileops.h \
    $$PWD/filesystem.h \
    $$PWD/global.h \
    $$PWD/localfileops.h \
    $$PWD/message.h \
    $$PWD/messagehandler.h \
    $$PWD/messageserver.h \
    $$PWD/node.h \
    $$PWD/remotefileops.h

linux-g++* {
    ### For posix file ops
    SOURCES += $$PWD/localfileops_posix.cpp
    ### For FUSE
    SOURCES += $$PWD/filesystem_fuse.cpp
    DEFINES += "_FILE_OFFSET_BITS=64"
    LIBS += "-lfuse"
}

macx {
    ### For posix file ops
    SOURCES += $$PWD/localfileops_posix.cpp
    ### For Fuse4X
    SOURCES += $$PWD/filesystem_fuse.cpp
    DEFINES += "_FILE_OFFSET_BITS=64"
    DEFINES += "_DARWIN_USE_64_BIT_INODE=1"
    QMAKE_LFLAGS += "-L/usr/local/lib -lfuse4x"
}

win32 {
    ### For win32 file ops
    SOURCES += $$PWD/localfileops_win32.cpp
    ### For Dokan
    SOURCES += $$PWD/filesystem_dokan.cpp
}
