/**
 * Copyright (C) 2015 : Kathrin Hanauer, Olaf Leßenich
 *
 * This file is part of Taffy.
 *
 * Taffy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taffy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taffy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 *   https://github.com/xaikal/taffy
 *   taffy@xaikal.org
 */


#include "taffydb.h"

#include "model.query/query.h"

#include <QtSql>
#include <QDir>

#include "taffydb_impl.cpp"

#include <cstdlib>

namespace taffy {

const QString DB_FILE_NAME("/.taffy.db");

struct TaffyDB::Data {
    QString dbLocation;
    QSqlDatabase db;
};

TaffyDB::TaffyDB()
  : d(new Data)
{

}

TaffyDB::~TaffyDB()
{
    disconnect();
    delete d;
}

bool TaffyDB::connect()
{
#ifdef QT_DEBUG
    qDebug() << "Connect called.";
#endif

    if (d->db.isOpen()) {
        return false;
    }

    d->db = QSqlDatabase::addDatabase("QSQLITE");
    getDBLocation();
#ifdef QT_DEBUG
    qDebug() << "DB location is" << d->dbLocation;
#endif
    d->db.setDatabaseName(d->dbLocation);
    QSqlError error = initDb(d->db);

    if (error.type() != QSqlError::NoError) {
#ifdef QT_DEBUG
    qDebug() << error;
#endif
        return false;
    }

    return true;
}

bool TaffyDB::disconnect()
{
#ifdef QT_DEBUG
    qDebug() << "Disconnect called.";
#endif

    if (!d->db.isOpen()) {
        return false;
    }
    QString dbConnName(d->db.connectionName());

    d->db.close();
    d->db = QSqlDatabase();
    QSqlDatabase::removeDatabase(dbConnName);
    return true;
}

void TaffyDB::addTagToFile(Tag &tag, File &file)
{
    tagFile(file, tag);
}

void TaffyDB::removeTagFromFile(Tag &tag, File &file)
{
    untagFile(file, tag);
}

QList<std::shared_ptr<Tag> > TaffyDB::showTagsOfFile(File &file)
{
    return showTags(file);
}

QList<std::shared_ptr<File> > TaffyDB::listFilesWithTag(Tag &tag)
{
    return listFilesByTag(tag);
}

QString TaffyDB::getDBLocation()
{
    if (!d->dbLocation.isEmpty()) {
        return d->dbLocation;
    }

    QString dbPath = DB_FILE_NAME;
    bool useHomeDir = true;

    char *taffy_home = getenv("TAFFY_HOME");
    if (taffy_home != NULL && taffy_home[0] != '\0') {
        QDir taffyHome(taffy_home);
        if (taffyHome.exists()) {
            dbPath.prepend(taffyHome.absolutePath());
            useHomeDir = false;
        }
    }

    if (useHomeDir) {
        dbPath.prepend(QDir::homePath());
    }

    d->dbLocation = dbPath;

    return dbPath;
}

void TaffyDB::setDBLocation(const QString &path)
{
    d->dbLocation = path;
}

}
