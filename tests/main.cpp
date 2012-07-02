#include <QtCore>
#include <QtTest/QtTest>

#include "testmessagehandler.h"

#include "node.h"

#define _NAME_ "mobtests"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(_NAME_);

    int rc = 0;
    TestMessageHandler test1;
    rc = QTest::qExec(&test1, argc, argv) == 0 ? rc : -1;
    return rc;
}
