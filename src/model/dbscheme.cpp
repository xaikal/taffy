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

namespace taffy {

/* CREATE STATEMENTS */

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

/* FILE MANAGEMENT STATEMENTS */

const QLatin1String DB_FIND_FILE_BY_ID(
        "SELECT path FROM files WHERE id = :id");

const QLatin1String DB_FIND_FILE_BY_PATH(
        "SELECT id FROM files WHERE file_path = :path");

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

/* TAG MANAGEMENT STATEMENTS */

const QLatin1String DB_FIND_TAG_BY_ID(
        "SELECT tag FROM tags WHERE id = :id");

const QLatin1String DB_FIND_TAG_BY_NAME(
        "SELECT * FROM tags WHERE tag = :tag");

const QLatin1String DB_ADD_TAG(
        "INSERT INTO tags (tag,  since) "
        "VALUES           (:tag, datetime('now'))");

/* TAGGINGS */

const QLatin1String DB_LIST_FILES_BY_TAG(
        "SELECT tg.file "
        "FROM taggings AS tg, "
        "     tags     AS t "
        "WHERE tg.tag = t.id "
        "  AND t.tag  = :tag");

const QLatin1String DB_SHOW_TAGS_OF_FILE(
        "SELECT tg.tag "
        "FROM taggings AS tg, "
        "     files    AS f "
        "WHERE tg.file = f.id "
        "  AND f.file_path  = :path");

const QLatin1String DB_TAG_FILE_CHECK(
        "SELECT id"
        "FROM taggings "
        "WHERE file = :file "
        "AND   tag  = :tag");

const QLatin1String DB_TAG_FILE(
        "INSERT INTO taggings (file,   tag) "
        "VALUES                (:file, :tag)");

const QLatin1String DB_UNTAG_FILE(
        "DELETE FROM taggings"
        "WHERE file = :file "
        "AND   tag  = :tag");


/* ALL */

const QLatin1String DB_LAST_ROWID("SELECT last_insert_rowid()");

}







