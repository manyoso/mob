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
    stream << usage << "[-s|--scheduler] [-i|--iface=<name>]\n";
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

QNetworkAddressEntry firstIPv4Address(const QNetworkInterface& iface)
{
    if (!(iface.flags() & QNetworkInterface::IsUp))
        return QNetworkAddressEntry();

    if (!(iface.flags() & QNetworkInterface::IsRunning))
        return QNetworkAddressEntry();

    if (iface.flags() & QNetworkInterface::IsLoopBack)
        return QNetworkAddressEntry();

    QList<QNetworkAddressEntry> addresses = iface.addressEntries();
    foreach (QNetworkAddressEntry address, addresses) {
        if (address.ip().protocol() == QAbstractSocket::IPv4Protocol)
            return address;
    }
    return QNetworkAddressEntry();
}

QNetworkAddressEntry firstIPv4Address(const QString& name)
{
    if (!name.isEmpty())
        return firstIPv4Address(QNetworkInterface::interfaceFromName(name));

    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface iface, ifaces) {
        QNetworkAddressEntry address = firstIPv4Address(iface);
        if (!address.ip().isNull())
            return address;
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
    QString iface;

    foreach(QString string, arguments) {
        if (string == QLatin1String("-s") || string == QLatin1String("--scheduler"))
            scheduler = true;
        else if (string.startsWith("-i=") || string.startsWith("--iface="))
            iface = string.mid(string.indexOf('=') + 1);
        else if (string == QLatin1String("-h") || string == QLatin1String("--help"))
            usage();
        else if (string == QLatin1String("-v") || string == QLatin1String("--version"))
            version();
        else
            usage(1);
    }

    QNetworkAddressEntry address = firstIPv4Address(iface);
    if (address.ip().isNull()) {
        qDebug() << "ERROR: Could not find an up and running network interface that supports IPv4 on this machine!";
        exit(1);
    }

    Server server(address, scheduler, &app);
    return app.exec();
}
