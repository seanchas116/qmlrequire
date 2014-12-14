#include "qmlrequireplugin.h"
#include "require.h"
#include "environment.h"

#include <qqml.h>
#include <QQmlEngine>

QString dirName;
QHash<QQmlEngine *, Environment *> environments;

void QmlRequirePlugin::registerTypes(const char *uri)
{
    dirName = baseUrl().toLocalFile();
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<Require>(uri, 0, 1, "Require");
}

Environment *QmlRequirePlugin::environment(QQmlEngine *engine)
{
    if (environments.contains(engine)) {
        return environments[engine];
    }
    else {
        auto env = new Environment(engine, dirName, engine);
        environments[engine] = env;
        return env;
    }
}
