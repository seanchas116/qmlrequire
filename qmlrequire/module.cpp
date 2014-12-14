#include "module.h"

#include "resolve.h"
#include "environment.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJSValueIterator>
#include <QQmlEngine>

static QJSValue bindMethod(const QJSValue &obj, const char *name)
{
    auto method = obj.property(name);
    return method.property("bind").callWithInstance(method, {obj});
}

Module::Module(Environment *environment, const QString &fileName, QObject *parent) :
    QObject(parent),
    mEnvironment(environment),
    mExports(environment->engine()->newObject()),
    mFileName(fileName)
{
}

Module::~Module()
{
}

void Module::setExports(const QJSValue &exports)
{
    mExports = exports;
}

QString Module::resolveRequire(const QString &path) const
{
    auto file = resolve::require(path, mFileName, mEnvironment->builtins().keys());
    if (file.isEmpty()) {
        // TODO: throw errors (impossible with Qt public API only?)
        qWarning() << "cannot load module" << path << "from" << mFileName;
    }
    return file;
}

QJSValue Module::require(const QString &path)
{
    auto fileName = resolveRequire(path);
    if (fileName.isEmpty()) {
        return QJSValue();
    }

    auto module = mEnvironment->modules()->value(fileName, nullptr);

    if (!module) {
        module = new Module(mEnvironment, fileName, this);

        module->mParentModule = this;
        mChildModules << module;

        module->load();
    }

    return module->exports();
}

void Module::load()
{
    QFile file(mFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "cannot open file: " << mFileName;
        return;
    }

    auto header =  "(function (module, exports, require, __filename, __dirname, process, Buffer) {";
    auto content = QString::fromUtf8(file.readAll());
    auto footer = "})";

    auto scope = mEnvironment->engine()->evaluate(header + content + footer, mFileName, 0);

    auto module = mEnvironment->engine()->newQObject(this);
    auto require = bindMethod(module, "require");

    auto fileName = mFileName;
    auto dirName = QFileInfo(fileName).absolutePath();

    auto process = mEnvironment->builtins()["__process"];
    auto bufferClass = mEnvironment->builtins()["__Buffer"];

    mEnvironment->modules()->insert(mFileName, this);

    scope.call({module, exports(), require, fileName, dirName, process, bufferClass});
}
