#include "global.h"

namespace Global {

static Node* s_scheduler = 0;
Node* scheduler()
{
    return s_scheduler;
}

void setScheduler(Node* node)
{
    s_scheduler = node;
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

} // namespace Global
