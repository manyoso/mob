include($$PWD/../mob.pri)
include($$PWD/../common/common.pri)

TEMPLATE = app
TARGET = mob
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

include($$PWD/client.pri)

SOURCES += main.cpp
