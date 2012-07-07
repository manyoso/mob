INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/global.cpp \
    $$PWD/localfile.cpp \
    $$PWD/message.cpp \
    $$PWD/messagehandler.cpp \
    $$PWD/node.cpp \
    $$PWD/remotefile.cpp

HEADERS += \
    $$PWD/fileoperations.h \
    $$PWD/filesystem.h \
    $$PWD/global.h \
    $$PWD/localfile.h \
    $$PWD/message.h \
    $$PWD/messagehandler.h \
    $$PWD/node.h \
    $$PWD/remotefile.h

macx {
    ### For Fuse4X
    SOURCES += $$PWD/filesystem_fuse.cpp
    DEFINES += "_FILE_OFFSET_BITS=64"
    DEFINES += "_DARWIN_USE_64_BIT_INODE"
    QMAKE_LFLAGS += "-L/usr/local/lib -lfuse4x"
}

win32 {
    ### For Dokan
    SOURCES += $$PWD/filesystem_dokan.cpp
}
