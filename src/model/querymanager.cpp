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


#include "querymanager.h"

#include "taffydb.h"
#include "model.query/query.h"

#include <QList>
#include <QtDebug>

namespace taffy {

struct QueryManager::QueryManagerImpl {
    TaffyDB db;
    QList<Query*> queries;
};

QueryManager::QueryManager()
    : impl(new QueryManagerImpl)
{
    impl->db.connect();
}

QueryManager::~QueryManager()
{
    for (auto i = impl->queries.constBegin(); i != impl->queries.constEnd(); i++) {
        Query *query = *i;
        delete query;
    }

    delete impl;
}

bool QueryManager::acceptQuery(Query *query)
{
#ifdef QT_DEBUG
    qDebug() << "QueryManager: Accepted query" << query->print();
#endif

    impl->queries.append(query);
    return query->exec(&impl->db);
}

}
