#include <QtTest/QtTest>

struct TestMessageHandlerPrivate;
class TestMessageHandler: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void sendMessage();
    void sendLargeMessage();

private:
    TestMessageHandlerPrivate* d;
};
