#include "require.h"

#include "module.h"
#include "environment.h"
#include "qmlrequireplugin.h"

#include <QQmlProperty>
#include <QQmlEngine>
#include <QQmlContext>

Require::Require(QObject *parent) : QObject(parent)
{
}

Require::~Require()
{
}

void Require::setTarget(const QQmlProperty &property)
{
    auto context = QQmlEngine::contextForObject(property.object());
    auto env = QmlRequirePlugin::environment(context->engine());

    auto module = new Module(env, context->baseUrl().toLocalFile(), property.object());

    property.write(QVariant::fromValue(module->require(mFrom)));
}
