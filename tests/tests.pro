include($$PWD/../mob.pri)
include($$PWD/../common/common.pri)

TEMPLATE = app
TARGET = mobtests
DESTDIR = $$OUTPUT_DIR/bin
CONFIG += qtestlib
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp peer.cpp testmessagehandler.cpp
HEADERS += peer.h testmessagehandler.h
