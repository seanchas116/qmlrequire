#include "environment.h"
#include "module.h"
#include "resolve.h"

#include <QQmlEngine>
#include <QDir>
#include <QJSValueIterator>
#include <QJsonDocument>
#include <QDebug>

Environment::Environment(QQmlEngine *engine, const QString &dirName, QObject *parent) :
    QObject(parent),
    mEngine(engine),
    mDirName(dirName)
{
    loadBuiltins();
}

Environment::~Environment()
{

}

static QVariant loadJson(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "cannot open" << fileName;
        return {};
    }

    auto doc = QJsonDocument::fromJson(file.readAll());
    return doc.toVariant();
}

void Environment::loadBuiltins()
{
    QDir dir(mDirName);
    auto builtins = loadJson(dir.absoluteFilePath("builtins.json")).toMap();

    auto fakeIndex = dir.absoluteFilePath("index.js");

    for (auto i = builtins.begin(); i != builtins.end(); ++i) {
        auto nodeModule = i.key();
        auto shimModule = i.value().toString();

        auto fileName = resolve::require(shimModule, fakeIndex, mBuiltins.keys());
        auto module = new Module(this, fileName, this);
        module->load();
        mModules[nodeModule] = module;
        qDebug() << shimModule << module->exports().toString();
        mBuiltins[nodeModule] = module->exports();
    }
}
