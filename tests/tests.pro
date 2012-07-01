include($$PWD/../mob.pri)
include($$PWD/../protocol/protocol.pri)

TEMPLATE = app
TARGET = mobtests
DESTDIR = $$OUTPUT_DIR/bin
CONFIG += qtestlib
CONFIG -= app_bundle
QT += network

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp testmessagehandler.cpp
HEADERS += testmessagehandler.h
