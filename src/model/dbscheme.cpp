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

#include <QString>

#include <QtSql>

const QLatin1String DB_CREATE_TABLE_FILES(
        "CREATE TABLE files ( "
        "  id         INTEGER PRIMARY KEY, "
        "  file_path  VARCHAR NOT NULL, "
        "  file_size  INTEGER, "
        "  file_hash  VARCHAR, "
        "  file_mtime DATETIME,"
        "  since      DATETIME"
        ")");

const QLatin1String DB_CREATE_TABLE_TAGS(
        "CREATE TABLE tags ( "
        "  id         INTEGER PRIMARY KEY, "
        "  tag        VARCHAR NOT NULL, "
        "  since      DATETIME"
        ")");


const QLatin1String DB_CREATE_TABLE_TAGGINGS(
        "CREATE TABLE taggings ( "
        "  id         INTEGER PRIMARY KEY, "
        "  file       INTEGER NOT NULL, "
        "  tag        INTEGER NOT NULL, "
        "  since      DATETIME"
        ")");

const QLatin1String DB_FIND_FILE_BY_PATH(
        "SELECT * FROM files WHERE file_path = :path");

const QLatin1String DB_DISCOVER_FILE(
        "INSERT INTO files (file_path, file_size, file_hash, file_mtime, since) "
        "VALUES            (:path,     :size,     :hash,     :mtime,     datetime('now'))");

const QLatin1String DB_FIND_FILE_BY_TAG(
        "SELECT tg.file "
        "FROM taggings AS tg, "
        "     tags     AS t "
        "WHERE tg.tag = t.id"
        "  AND t.tag  = :tag");

const QLatin1String DB_ADD_TAG(
        "INSERT INTO tags (tag,  since) "
        "VALUES           (:tag, datetime('now'))");

const QLatin1String DB_LIST_TAGS_OF_FILE(
        "SELECT tg.tag "
        "FROM taggings AS tg, "
        "     files    AS f "
        "WHERE tg.file = f.id"
        "  AND f.file_path  = :path");


QSqlError checkTable(const QString &table, const QStringList &tables, const QLatin1String &createStatement) {
    if (!tables.contains(table, Qt::CaseInsensitive)) {
        QSqlQuery query;
        if (!query.exec(createStatement)) {
            return query.lastError();
        }
    }
    return QSqlError();
}

QSqlError initDb(QSqlDatabase &db) {
    if (!db.open()) {
        return db.lastError();
    }

    QStringList dbTables = db.tables();
    QSqlError error = checkTable("files", dbTables, DB_CREATE_TABLE_FILES);
    if (error.type() != QSqlError::NoError) {
        return error;
    }
    error = checkTable("tags", dbTables, DB_CREATE_TABLE_TAGS);
    if (error.type() != QSqlError::NoError) {
        return error;
    }
    error = checkTable("taggings", dbTables, DB_CREATE_TABLE_TAGGINGS);
    if (error.type() != QSqlError::NoError) {
        return error;
    }

    return QSqlError();
}

QSqlQuery findFile(const QString &filePath) {
    QSqlQuery query;
    query.prepare(DB_FIND_FILE_BY_PATH);
    query.bindValue(":path", filePath);
    query.exec();
    return query;
}

QSqlQuery discoverFile(const QFileInfo &file) {
    QSqlQuery query;
    query.prepare(DB_DISCOVER_FILE);
    query.bindValue(":path", file.absoluteFilePath());
    query.bindValue(":size", file.size());
    query.bindValue(":hash", "");
    query.bindValue(":size", file.lastModified());
    query.exec();
    return query;
}

QSqlQuery findTaggedFiles(const QString &tag) {
    QSqlQuery query;
    query.prepare(DB_FIND_FILE_BY_TAG);
    query.bindValue(":tag", tag);
    query.exec();
    return query;
}

QSqlQuery addTag(const QString &tag) {
    QSqlQuery query;
    query.prepare(DB_ADD_TAG);
    query.bindValue(":tag", tag);
    query.exec();
    return query;
}

QSqlQuery listFileTags(const QFileInfo &file) {
    QSqlQuery query;
    query.prepare(DB_LIST_TAGS_OF_FILE);
    query.bindValue(":path", file.absoluteFilePath());
    query.exec();
    return query;
}
