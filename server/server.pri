DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD $$PWD/../thirdparty/sigar

SOURCES += $$PWD/server.cpp $$PWD/sysinfo.cpp
HEADERS += $$PWD/server.h
LIBS += $$OUTPUT_DIR/lib/libsigar.a

macx {
    ### For sigar
    QMAKE_LFLAGS += "-pthread -liconv -framework CoreServices -framework IOKit"
}

win32 {
    ### For sigar
    LIBS += libws2_32 libnetapi32 libversion
}
