#include "resolve.h"
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// http://nodejs.org/api/modules.html#modules_all_together

namespace resolve {

QString loadAsFile(const QString &path)
{
    if (QFileInfo(path).isFile()) {
        return path;
    }
    if (QFileInfo(path + ".js").isFile()) {
        return path + ".js";
    }
    if (QFileInfo(path + ".json").isFile()) {
        return path + ".json";
    }
    return QString();
}

QString loadAsDirectory(const QString &path)
{
    auto packageJson = QDir(path).filePath("package.json");
    if (QFileInfo(packageJson).isFile()) {
        QFile file(packageJson);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "cannot open file" << packageJson;
            return QString();
        }
        auto json = QJsonDocument::fromJson(file.readAll());
        auto main = json.object()["main"];
        if (main.isString()) {
            return loadAsFile(QDir(path).filePath(main.toString()));
        }
    }

    auto indexJs = QDir(path).filePath("index.js");
    if (QFileInfo(indexJs).isFile()) {
        return indexJs;
    }
    auto indexJson = QDir(path).filePath("index.json");
    if (QFileInfo(indexJson).isFile()) {
        return indexJson;
    }
    return QString();
}

QStringList nodeModulesPaths(const QString &start)
{
    QStringList dirs;
    QString remaining = start;

    while (true) {
        QFileInfo info(remaining);
        if (info.isRoot())  break;
        if (info.fileName() != "node_modules") {
            auto dir = QDir(remaining).filePath("node_modules");
            dirs << dir;
        }
        remaining = info.absolutePath();
    }

    return dirs;
}

QString loadNodeModules(const QString &path, const QString &start)
{
    auto dirs = nodeModulesPaths(start);
    for (auto dir : dirs) {
        auto fullPath = QDir(dir).filePath(path);

        auto file = loadAsFile(fullPath);
        if (!file.isEmpty()) {
            return file;
        }

        file = loadAsDirectory(fullPath);
        if (!file.isEmpty()) {
            return file;
        }
    }
    return QString();
}

QString require(const QString &path, const QString &from, const QStringList &coreModules)
{
    if (coreModules.contains(path)) {
        return path;
    }

    if (path.mid(0, 2) == "./" || path.mid(0, 1) == "/" || path.mid(0, 3) == "../") {
        auto fullPath = QFileInfo(from).dir().absoluteFilePath(path);

        auto file = loadAsFile(fullPath);
        if (!file.isEmpty()) {
            return file;
        }

        file = loadAsDirectory(fullPath);
        if (!file.isEmpty()) {
            return file;
        }
    }

    return loadNodeModules(path, QFileInfo(from).absolutePath());
}

}
