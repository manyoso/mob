#include "testnode.h"

#include "node.h"
#include "peer.h"

void TestNode::testSendNodeMessage()
{
    Peer peer1(1111, 2222);
    QVERIFY(peer1.readPort() == 1111);
    QVERIFY(peer1.writePort() == 2222);

    Peer peer2(2222, 1111);
    QVERIFY(peer2.readPort() == 2222);
    QVERIFY(peer2.writePort() == 1111);

    QSharedPointer<MessageHandler> handler(new MessageHandler);
    peer2.installMessageHandler(handler, MessageFilter());

    Node node(true /*isLocal*/, QHostAddress::LocalHost);
    NodeInfo msg;
    msg.setAddress(node.address().toIPv4Address());
    msg.setScheduler(node.scheduler());
    QVERIFY(peer1.sendMessage(msg) == true);

    QVERIFY(handler->waitForMessage() == true);
    QVERIFY(handler->messageCount() == 1);
    QSharedPointer<Message> out = handler->dequeueMessage();
    QVERIFY(out->type() == msg.type());
    if (out->type() == msg.type()) {
        QSharedPointer<NodeInfo> info = out.staticCast<NodeInfo>();
        QVERIFY(info->address() == node.address().toIPv4Address());
        QVERIFY(info->scheduler() == node.scheduler());
    }
}
