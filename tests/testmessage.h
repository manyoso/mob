#include <QtTest/QtTest>

class TestMessage: public QObject {
    Q_OBJECT
private slots:
    void testSendMessage();
    void testSendLargeMessage();
    void testMessageWaitTimeout();
    void testMessageFilter();
    void testMessageSize();
};
