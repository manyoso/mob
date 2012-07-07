include($$PWD/build.pri)

QT += core
QT -= gui

CONFIG += qt thread warn_on
CONFIG -= debug rtti exceptions

QMAKE_CXXFLAGS += -Werror -fno-rtti -fno-exceptions

DEBUG_MODE = $$(DEBUG_MODE)
contains(DEBUG_MODE, 1) {
  CONFIG += debug
}
