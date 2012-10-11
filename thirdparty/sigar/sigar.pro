include($$PWD/../../build.pri)

TEMPLATE = lib
TARGET = sigar
CONFIG += staticlib warn_off
CONFIG -= qt debug
DESTDIR = $$OUTPUT_DIR/lib

SOURCES += \
    sigar.c \
    sigar_cache.c \
    sigar_fileinfo.c \
    sigar_format.c \
    sigar_getline.c \
    sigar_ptql.c \
    sigar_signal.c \
    sigar_util.c

linux-g++* {
    INCLUDEPATH += os/linux
    SOURCES += os/linux/linux_sigar.c
}

macx {
    DEFINES += DARWIN
    INCLUDEPATH += os/darwin
    SOURCES += os/darwin/darwin_sigar.c
}

win32 {
    ## make sure we only use the smallest set of
    ## headers on win32. Otherwise we get clashes
    ## between winsock2.h and winsock.h
    DEFINES += WIN32_LEAN_AND_MEAN

    # force the correct version for the redist manifest
    DEFINES += "_BIND_TO_CURRENT_MFC_VERSION=1"
    DEFINES += "_BIND_TO_CURRENT_CRT_VERSION=1"
    # turn off security warnings for system calls
    DEFINES += CRT_SECURE_NO_WARNINGS

    INCLUDEPATH += os/win32
    SOURCES += \
        os/win32/peb.c  \
        os/win32/win32_sigar.c
}
