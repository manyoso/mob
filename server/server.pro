include($$PWD/../mob.pri)
include($$PWD/../common/common.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += . $$PWD/../thirdparty/sigar

SOURCES += main.cpp server.cpp sysinfo.cpp
HEADERS += filesystem.h server.h
LIBS += $$OUTPUT_DIR/lib/libsigar.a

macx {
    ### For Fuse4X
    SOURCES += filesystem_fuse.cpp
    DEFINES += "_FILE_OFFSET_BITS=64"
    DEFINES += "_DARWIN_USE_64_BIT_INODE"

    ### For sigar
    QMAKE_LFLAGS += "-L/usr/local/lib -lfuse4x -pthread -liconv -framework CoreServices -framework IOKit"
}

win32 {
    ### For Dokan
    SOURCES += filesystem_dokan.cpp

    ### For sigar
    LIBS += libws2_32 libnetapi32 libversion
}
