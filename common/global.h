#ifndef global_h
#define global_h

#include <QtNetwork/QNetworkInterface>
class Node;

namespace Global {

extern Node* scheduler();
extern void setScheduler(Node*);
extern QNetworkAddressEntry firstIPv4Address(const QNetworkInterface& iface);
extern QNetworkAddressEntry firstIPv4Address(const QString& name);

} // namespace Global

#endif // global_h
