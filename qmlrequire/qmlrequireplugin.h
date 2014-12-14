#ifndef QML_REQUIRE_PLUGIN_H
#define QML_REQUIRE_PLUGIN_H

#include <QQmlExtensionPlugin>

class Environment;

class QmlRequirePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);

    static Environment *environment(QQmlEngine *engine);
};

#endif // QML_REQUIRE_PLUGIN_H
