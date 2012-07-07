#include <QtCore>
#include <QtTest/QtTest>

#include "testmessage.h"
#include "testnode.h"

#define _NAME_ "mobtests"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(_NAME_);

    int rc = 0;
    TestMessage test1;
    rc = QTest::qExec(&test1, argc, argv) == 0 ? rc : -1;

    TestNode test2;
    rc = QTest::qExec(&test2, argc, argv) == 0 ? rc : -1;
    return rc;
}
