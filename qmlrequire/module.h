#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QJSValue>

class Environment;

class Module : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QJSValue exports WRITE setExports READ exports)
    Q_PROPERTY(bool loaded READ isLoaded)
    Q_PROPERTY(QString filename READ fileName)
public:
    explicit Module(Environment *environment, const QString &fileName, QObject *parent);
    ~Module();

    void setExports(const QJSValue &exports);
    QJSValue exports() const { return mExports; }

    QString fileName() const { return mFileName; }
    bool isLoaded() const { return mIsLoaded; }
    Module *parentModule() const { return mParentModule; }
    QList<Module *> childModules() const { return mChildModules; }

    Q_INVOKABLE QJSValue require(const QString &path);
    Q_INVOKABLE QString resolveRequire(const QString &module) const;

    void load();

signals:

public slots:

private:

    Environment *mEnvironment = nullptr;
    QJSValue mExports;
    QString mFileName;
    bool mIsLoaded = false;
    Module *mParentModule = nullptr;
    QList<Module *> mChildModules;

    static QHash<QString, Module *> mAllModules;
};

#endif // MODULE_H
