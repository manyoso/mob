include($$PWD/../mob.pri)
include($$PWD/../protocol/protocol.pri)

TEMPLATE = app
TARGET = mob
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp client.cpp
HEADERS += client.h