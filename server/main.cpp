#include <QtCore>
#include <QtNetwork/QNetworkInterface>

#include "server.h"

#define _NAME_ "mobd"
#define _VERSION_ "0.1"

void usage(int exitCode = 0)
{
    QString usage = QLatin1String("usage: ") + QLatin1String(_NAME_) + QLatin1String(" ");
    QString space = usage;
    space = space.fill(' ');

    QTextStream stream(stdout);
    stream << usage << "[-s|--scheduler]\n";
    stream << space << "[-h|--help] [-v|--version]\n";
    stream << "\n";
    stream << "Arguments:\n";
    stream << "\n";
    stream.flush();
    exit(exitCode);
}

void version()
{
    QTextStream stream(stdout);
    stream << _NAME_ << " version " << _VERSION_<< "\n";
    stream.flush();
    exit(0);
}

QNetworkAddressEntry firstIpv4Address()
{
    // Return the - first up and running, non-loopback - local address that
    // supports the IPv4 protocol
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface iface, ifaces) {
        if (!(iface.flags() & QNetworkInterface::IsLoopBack)
            && iface.flags() & QNetworkInterface::IsUp
            && iface.flags() & QNetworkInterface::IsRunning) {

            QList<QNetworkAddressEntry> addresses = iface.addressEntries();
            foreach (QNetworkAddressEntry address, addresses) {
                if (address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    return address;
            }
        }
    }

    return QNetworkAddressEntry();
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments = app.arguments();
    Q_ASSERT(!arguments.isEmpty());
    arguments.removeFirst(); // remove the app name

    bool scheduler = false;

    foreach(QString string, arguments) {
        if (string == QLatin1String("-s") || string == QLatin1String("--scheduler"))
            scheduler = true;
        else if (string == QLatin1String("-h") || string == QLatin1String("--help"))
            usage();
        else if (string == QLatin1String("-v") || string == QLatin1String("--version"))
            version();
        else
            usage(1);
    }

    QNetworkAddressEntry address = firstIpv4Address();
    if (address.ip().isNull()) {
        qDebug() << "ERROR: Could not find an up and running network interface that supports IPv4 on this machine!";
        exit(1);
    }

    Server server(address, &app);
    return app.exec();
}
