#ifndef sysinfo_h
#define sysinfo_h

#include <QtCore/QList>

class SysInfo {
public:
    static QList<double> loadAverage();
};

#endif // sysinfo_h
