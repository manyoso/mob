#include "sysinfo.h"

#include "sigar.h"

#include <QtCore/QDebug>

QList<double> SysInfo::loadAverage()
{
#ifdef Q_OS_WIN32
    return QList<double>() << 1.0 << 1.0 << 1.0;
#else
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_loadavg_t load;
    int rc = sigar_loadavg_get(sigar, &load);
    if (rc != SIGAR_OK) {
        qDebug() << "sigar_loadavg_get error:" << rc << "(" << sigar_strerror(sigar, rc) << ")";
    }
    sigar_close(sigar);
    return QList<double>() << load.loadavg[0] << load.loadavg[1] << load.loadavg[2];
#endif
}
