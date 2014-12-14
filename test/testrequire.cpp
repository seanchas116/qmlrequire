#include <QString>
#include <QtTest>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QJSValue>
#include "../qmlrequire/environment.h"

class TestRequire : public QObject
{
    Q_OBJECT

public:
    TestRequire()
    {
        mEngine = new QQmlEngine();
        auto filePath = QDir(__FILE__).filePath("../../qmlrequire");
        mEnvironment = new Environment(mEngine, QFileInfo(filePath).absoluteFilePath(), this);
    }

private slots:
    void testBuiltins()
    {
        QJSValue path = mEnvironment->builtins()["path"];
        qDebug() << path.toString();
        QCOMPARE(path.property("join").call({"foo/bar/", "baz"}).toString(), QString("foo/bar/baz"));
    }

private:

    QQmlEngine *mEngine;
    Environment *mEnvironment;
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    TestRequire tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "testrequire.moc"
