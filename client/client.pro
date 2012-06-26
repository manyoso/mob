include($$PWD/../mob.pri)

TEMPLATE = app
TARGET = mob
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp
