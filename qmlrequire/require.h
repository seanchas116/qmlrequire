#ifndef REQUIRE_H
#define REQUIRE_H

#include <QObject>
#include <QQmlPropertyValueSource>

class Require : public QObject, public QQmlPropertyValueSource
{
    Q_OBJECT
    Q_INTERFACES(QQmlPropertyValueSource)
    Q_PROPERTY(QString from MEMBER mFrom)

public:
    explicit Require(QObject *parent = 0);
    ~Require();

    void setTarget(const QQmlProperty &property) override;

private:
    QString mFrom;
};

#endif // REQUIRE_H
