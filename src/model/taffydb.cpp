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
#include "dbscheme.cpp"

#include "model.query/query.h"

#include <QtSql>

struct TaffyDB::TaffyDBImpl {
    QSqlDatabase db;
};

TaffyDB::TaffyDB()
  : impl(new TaffyDBImpl)
{

}

TaffyDB::~TaffyDB()
{
    disconnect();
    delete impl;
}

bool TaffyDB::connect()
{
#ifdef QT_DEBUG
    qDebug() << "Connect called.";
#endif

    if (impl->db.isOpen()) {
        return false;
    }

    impl->db = QSqlDatabase::addDatabase("QSQLITE");
    impl->db.setDatabaseName(":memory:");
    QSqlError error = initDb(impl->db);

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

    if (!impl->db.isOpen()) {
        return false;
    }
    QString dbConnName(impl->db.connectionName());

    impl->db.close();
    impl->db = QSqlDatabase();
    QSqlDatabase::removeDatabase(dbConnName);
    return true;
}

