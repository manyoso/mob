include($$PWD/../mob.pri)
include($$PWD/../protocol/protocol.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp server.cpp
HEADERS += server.h
LIBS += $$OUTPUT_DIR/lib/libsigar.a
