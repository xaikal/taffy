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

#include "dbscheme.cpp"

#include "model.data/file.h"
#include "model.data/tag.h"

#include <QtSql>

#include <memory>
#include <stdexcept>

namespace taffy {

void runQuery(QSqlQuery &query);

QSqlError checkTable(const QString &table, const QStringList &tables, const QLatin1String &createStatement);
QSqlError initDb(QSqlDatabase &db);

void setDbIdentifier(DbObject *o, int id);

bool findFile(File &file);
void addFile(File &file);
bool updateFile(File &file, bool checkExistence = true);
File *createFileFromId(int id);

bool findTag(Tag &tag);
void addTag(Tag &tag);
Tag *createTagFromId(int id);

QList<std::shared_ptr<File> > listFilesByTag(Tag &tag);
QList<std::shared_ptr<Tag> > showTags(File &file);

bool hasTag(File &file, Tag &tag);
void tagFile(File &file, Tag &tag);
void untagFile(File &file, Tag &tag);

// BEGIN DEFINITIONS

void runQuery(QSqlQuery &query) {
#ifdef QT_DEBUG
    qDebug() << "Running query:" << query.lastQuery();
#endif
    if (!query.exec()) {
        QSqlError error = query.lastError();
        throw std::runtime_error(qPrintable(error.text()));
    }
}

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

void setDbIdentifier(DbObject *o, int id) {
    int curId = o->getIdentifier();
    if (curId == DbObject::NO_ID) {
        o->setIdentifier(id);
    } else if (curId != id) {
        throw std::runtime_error("IDs do not match.");
    }
}

File *createFileFromId(int id) {
    QSqlQuery query;
    query.prepare(DB_FIND_FILE_BY_ID);
    query.bindValue(":id", id);
    runQuery(query);

    // Is file in db (i.e., result set non-empty)?
    if (query.next()) {
        QString path = query.value(0).toString();

        File *file = new File(path);
        file->setIdentifier(id);
        return file;
    }

    return 0;
}

bool findFile(File &file) {
    QSqlQuery query;
    query.prepare(DB_FIND_FILE_BY_PATH);
    query.bindValue(":path", file.getPath());
    runQuery(query);

    // Is file in db (i.e., result set non-empty)?
    if (query.next()) {
        int id = query.value(0).toInt();

        // Is size of result set even greater than one?
        // check BEFORE(!) modifying file object
        if (query.next()) {
            throw std::runtime_error("Duplicate entries in DB.");
        }

        setDbIdentifier(&file, id);

        return true;
    }

    return false;
}

void addFile(File &file) {
    if (findFile(file)) {
        updateFile(file, false);
        return;
    }

    QSqlQuery query;
    query.prepare(DB_ADD_FILE);
    query.bindValue(":path", file.getPath());
    query.bindValue(":size", file.getSize());
    query.bindValue(":hash", file.getHash());
    query.bindValue(":mtime", file.getLastModified());
    runQuery(query);

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

bool updateFile(File &file, bool checkExistence) {
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
    runQuery(query);
    return true;
}

Tag *createTagFromId(int id) {
    QSqlQuery query;
    query.prepare(DB_FIND_TAG_BY_ID);
    query.bindValue(":id", id);
    runQuery(query);

    // Is tag in db (i.e., result set non-empty)?
    if (query.next()) {
        QString tagName = query.value(0).toString();

        Tag *tag = new Tag(tagName);
        tag->setIdentifier(id);
        return tag;
    }

    return 0;
}

bool findTag(Tag &tag) {
    QSqlQuery query;
    query.prepare(DB_FIND_TAG_BY_NAME);
    query.bindValue(":tag", tag.getTag());
    runQuery(query);

    // Is tag in db (i.e., result set non-empty)?
    if (query.next()) {
        int id = query.value(0).toInt();

        // Is size of result set even greater than one?
        // check BEFORE(!) modifying tag object
        if (query.next()) {
            throw std::runtime_error("Duplicate entries in DB.");
        }

        setDbIdentifier(&tag, id);

        return true;
    }

    return false;
}

void addTag(Tag &tag) {
    if (findTag(tag)) {
        return;
    }

    QSqlQuery query;
    query.prepare(DB_ADD_TAG);
    query.bindValue(":tag", tag.getTag());
    runQuery(query);

    query.prepare(DB_LAST_ROWID);
    runQuery(query);

    if (query.next()) {
        int id = query.value(0).toInt();
        tag.setIdentifier(id);
    } else {
        throw std::runtime_error("Could not retrieve last inserted row id.");
    }
}

QList<std::shared_ptr<File> > listFilesByTag(Tag &tag) {
    QList<std::shared_ptr<File> > files;

    // if tag is unkown, no files can be tagged...
    if (!findTag(tag)) {
        return files;
    }

    QSqlQuery query;
    query.prepare(DB_LIST_FILES_BY_TAG);
    query.bindValue(":tag", tag.getTag());
    runQuery(query);

    while (query.next()) {
        int fileId = query.value(0).toInt();
        File *file = createFileFromId(fileId);
        if (file == 0) {
            QString msg("Cannot find file with id %1.");
            msg.arg(fileId);
            throw std::runtime_error(qPrintable(msg));
        }
        files.push_back(std::shared_ptr<File>(file));
    }
    return files;
}

QList<std::shared_ptr<Tag> > showTags(File &file) {
    QList<std::shared_ptr<Tag> > tags;

    // if file is unkown, no tags can be known
    if (!findFile(file)) {
        return tags;
    }

    QSqlQuery query;
    query.prepare(DB_SHOW_TAGS_OF_FILE);
    query.bindValue(":path", file.getPath());
    runQuery(query);

    while (query.next()) {
        int tagId = query.value(0).toInt();
        Tag *tag = createTagFromId(tagId);
        if (tag == 0) {
            QString msg("Cannot find tag with id %1.");
            msg.arg(tagId);
            throw std::runtime_error(qPrintable(msg));
        }
        tags.push_back(std::shared_ptr<Tag>(tag));
    }
    return tags;
}

bool hasTag(File &file, Tag &tag) {
    if (!findFile(file) || !findTag(tag)) {
        return false;
    }

    QSqlQuery query;
    query.prepare(DB_TAG_FILE_CHECK);
    query.bindValue(":file", file.getIdentifier());
    query.bindValue(":tag", tag.getIdentifier());
    runQuery(query);

    if (query.next()) {
        if (query.next()) {
            throw std::runtime_error("Duplicate entries in DB.");
        }
        return true;
    }
    return false;
}

void tagFile(File &file, Tag &tag) {
    if (hasTag(file, tag)) {
        return;
    }
    addFile(file);
    addTag(tag);

    QSqlQuery query;
    query.prepare(DB_TAG_FILE);
    query.bindValue(":file", file.getIdentifier());
    query.bindValue(":tag", tag.getIdentifier());
    runQuery(query);
}

void untagFile(File &file, Tag &tag) {
    if (!hasTag(file, tag)) {
        return;
    }
    QSqlQuery query;
    query.prepare(DB_UNTAG_FILE);
    query.bindValue(":file", file.getIdentifier());
    query.bindValue(":tag", tag.getIdentifier());
    runQuery(query);
}

}
