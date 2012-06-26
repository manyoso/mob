include($$PWD/../mob.pri)

TEMPLATE = app
TARGET = mobd
DESTDIR = $$OUTPUT_DIR/bin

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp
