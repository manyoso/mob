#include <QtTest/QtTest>

struct TestMessagePrivate;
class TestMessage: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSendMessage();
    void testSendLargeMessage();
    void testMessageWaitTimeout();
    void testMessageFilter();
    void testMessageSize();

private:
    TestMessagePrivate* d;
};
