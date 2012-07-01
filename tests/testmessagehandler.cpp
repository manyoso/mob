#include "testmessagehandler.h"

void TestMessageHandler::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}
