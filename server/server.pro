include($$PWD/../mob.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin
CONFIG -= app_bundle

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp
