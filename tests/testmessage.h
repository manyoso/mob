#include <QtTest/QtTest>

struct TestMessagePrivate;
class TestMessage: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void sendMessage();
    void sendLargeMessage();

private:
    TestMessagePrivate* d;
};
