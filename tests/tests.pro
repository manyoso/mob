include($$PWD/../mob.pri)
include($$PWD/../common/common.pri)
include($$PWD/../server/server.pri)
include($$PWD/../client/client.pri)

TEMPLATE = app
TARGET = mobtests
DESTDIR = $$OUTPUT_DIR/bin
CONFIG += qtestlib
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp peer.cpp testnode.cpp testmessage.cpp
HEADERS += peer.h testnode.h testmessage.h
