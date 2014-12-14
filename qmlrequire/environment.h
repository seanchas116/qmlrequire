#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>
#include <QJSValue>
#include <QHash>
#include <QStringList>

class Module;

class QQmlEngine;

class Environment : public QObject
{
    Q_OBJECT
public:
    explicit Environment(QQmlEngine *engine, const QString &dirName, QObject *parent);
    ~Environment();

    QQmlEngine *engine() { return mEngine; }
    QString dirName() const { return mDirName; }
    QHash<QString, QJSValue> builtins() const { return mBuiltins; }

    QHash<QString, Module *> *modules() { return &mModules; }

private:

    void loadBuiltins();

    QQmlEngine *mEngine;
    QString mDirName;
    QHash<QString, QJSValue> mBuiltins;
    QHash<QString, Module *> mModules;
};

#endif // ENVIRONMENT_H
