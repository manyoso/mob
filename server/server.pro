include($$PWD/../mob.pri)
include($$PWD/../common/common.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle
QT += network

include($$PWD/server.pri)

SOURCES += main.cpp
