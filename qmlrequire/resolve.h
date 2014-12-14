#ifndef RESOLVE_H
#define RESOLVE_H

class QString;
class QStringList;

namespace resolve {

QString require(const QString &path, const QString &from, const QStringList &coreModules);

}

#endif // RESOLVE_H
