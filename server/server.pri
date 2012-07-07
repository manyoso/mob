DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD $$PWD/../thirdparty/sigar

SOURCES += $$PWD/server.cpp $$PWD/sysinfo.cpp
HEADERS += $$PWD/filesystem.h $$PWD/server.h
LIBS += $$OUTPUT_DIR/lib/libsigar.a

macx {
    ### For Fuse4X
    SOURCES += $$PWD/filesystem_fuse.cpp
    DEFINES += "_FILE_OFFSET_BITS=64"
    DEFINES += "_DARWIN_USE_64_BIT_INODE"

    ### For sigar
    QMAKE_LFLAGS += "-L/usr/local/lib -lfuse4x -pthread -liconv -framework CoreServices -framework IOKit"
}

win32 {
    ### For Dokan
    SOURCES += $$PWD/filesystem_dokan.cpp

    ### For sigar
    LIBS += libws2_32 libnetapi32 libversion
}
