include($$PWD/../mob.pri)
include($$PWD/../protocol/protocol.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += . $$PWD/../thirdparty/sigar

SOURCES += main.cpp server.cpp sysinfo.cpp
HEADERS += server.h
LIBS += $$OUTPUT_DIR/lib/libsigar.a

macx {
    ### For sigar
    QMAKE_LFLAGS += "-framework CoreServices -framework IOKit"
}

win32 {
    ### For sigar
    LIBS += libws2_32 libnetapi32 libversion
}
