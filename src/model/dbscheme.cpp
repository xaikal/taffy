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

#include "model.data/file.h"

#include <QString>

#include <QtSql>

#include <stdexcept>

namespace taffy {

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

const QLatin1String DB_ADD_FILE(
        "INSERT INTO files (file_path, file_size, file_hash, file_mtime, since) "
        "VALUES            (:path,     :size,     :hash,     :mtime,     datetime('now'))");

const QLatin1String DB_UPDATE_FILE(
        "UPDATE files SET "
        "  file_path  = :path, "
        "  file_size  = :size, "
        "  file_hash  = :hash, "
        "  file_mtime = :mtime, "
        "  since      = datetime('now') "
        "WHERE id = :id");

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

const QLatin1String DB_LAST_ROWID("last_insert_rowid()");



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

struct TaffyDB::Data {
    QSqlDatabase db;

    bool findFile(File &file);
    void addFile(File &file);
    bool updateFile(File &file, bool checkExistence = true);
};

bool TaffyDB::Data::findFile(File &file) {
    QSqlQuery query;
    query.prepare(DB_FIND_FILE_BY_PATH);
    query.bindValue(":path", file.getPath());
    if (!query.exec()) {
        QSqlError error = query.lastError();
        throw std::runtime_error(qPrintable(error.text()));
    }

    // Is file in db (i.e., result set non-empty)?
    if (query.next()) {
        int id = query.value(0).toInt();
        int fileId = file.getIdentifier();
        if (fileId == DbObject::NO_ID) {
            file.setIdentifier(id);
            return true;
        } else if (fileId == id) {
            return true;
        } else {
            throw std::runtime_error("IDs do not match.");
        }

        // Is size of result set even greater than one?
        if (query.next()) {
            throw std::runtime_error("Duplicate entries in DB.");
        }
    }

    return false;
}

void TaffyDB::Data::addFile(File &file) {
    if (findFile(file)) {
        //updateFile(file, false);
        return;
    }

    QSqlQuery query;
    query.prepare(DB_ADD_FILE);
    query.bindValue(":path", file.getPath());
    query.bindValue(":size", file.getSize());
    query.bindValue(":hash", file.getHash());
    query.bindValue(":mtime", file.getLastModified());
    if (!query.exec()) {
        QSqlError error = query.lastError();
        throw std::runtime_error(qPrintable(error.text()));
    }

    if (!query.exec(DB_LAST_ROWID)) {
        QSqlError error = query.lastError();
        throw std::runtime_error(qPrintable(error.text()));
    }

    if (query.next()) {
        int id = query.value(0).toInt();
        file.setIdentifier(id);
    } else {
        throw std::runtime_error("Could not retrieve last inserted row id.");
    }
}

bool TaffyDB::Data::updateFile(File &file, bool checkExistence) {
    if (checkExistence && !findFile(file)) {
        return false;
    } else {
        if (file.getIdentifier() == DbObject::NO_ID) {
            throw std::runtime_error("File has no id set.");
        }
    }

    QSqlQuery query;
    query.prepare(DB_UPDATE_FILE);
    query.bindValue(":id", file.getIdentifier());
    query.bindValue(":path", file.getPath());
    query.bindValue(":size", file.getSize());
    query.bindValue(":hash", file.getHash());
    query.bindValue(":mtime", file.getLastModified());
    if (!query.exec()) {
        QSqlError error = query.lastError();
        throw std::runtime_error(qPrintable(error.text()));
    }
    return true;
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

}
