include($$PWD/build.pri)

QT += core
QT -= gui

CONFIG += qt warn_on
CONFIG -= debug

QMAKE_CXXFLAGS += -Werror

DEBUG_MODE = $$(DEBUG_MODE)
contains(DEBUG_MODE, 1) {
  CONFIG += debug
}
