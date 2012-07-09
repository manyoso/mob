#include <QtTest/QtTest>

struct TestMessagePrivate;
class TestMessage: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void sendMessage();
    void sendLargeMessage();
    void testMessageWaitTimeout();

private:
    TestMessagePrivate* d;
};
