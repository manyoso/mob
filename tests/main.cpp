#include <QtCore>
#include <QtTest/QtTest>

#include "testmessagehandler.h"

#include "node.h"

static Node* s_scheduler = 0;
Node* scheduler()
{
    return s_scheduler;
}

int main(int argc, char* argv[])
{
    int rc = 0;
    TestMessageHandler test1;
    rc = QTest::qExec(&test1, argc, argv) == 0 ? rc : -1;
    return rc;
}
