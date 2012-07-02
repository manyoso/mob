#include <QtTest/QtTest>

struct TestMessageHandlerPrivate;
class TestMessageHandler: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void sendMessage();
private:
    TestMessageHandlerPrivate* d;
};
