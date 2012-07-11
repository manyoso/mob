#include "testnode.h"

#include "node.h"
#include "peer.h"

void TestNode::sendNodeMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    Node node(true /*isLocal*/, QHostAddress::LocalHost);
    NodeInfo msg;
    msg.setAddress(node.address().toIPv4Address());
    msg.setScheduler(node.scheduler());
    QVERIFY(peer1.sendMessage(msg) == true);
    QVERIFY(peer2.waitForMessage() == true);

    QSharedPointer<Message> out = peer2.messageReceived();
    QVERIFY(out->type() == msg.type());
    if (out->type() == msg.type()) {
        QSharedPointer<NodeInfo> info = out.staticCast<NodeInfo>();
        QVERIFY(info->address() == node.address().toIPv4Address());
        QVERIFY(info->scheduler() == node.scheduler());
    }
}
