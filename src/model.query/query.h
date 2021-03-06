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


#ifndef QUERY_H
#define QUERY_H

#include <QString>
#include <QStringList>

namespace taffy {

class TaffyDB;

/*!
 * A query that is performed with Taffy.
 */
class Query
{
public:
    /*!
     * Creates a new Query.
     *
     * @param   files   A List of files to perform the query on.
     */
    explicit Query(const QStringList &files);
    virtual ~Query();

    /*!
     * Returns a string representation of the query.
     *
     * @return  string representation of the query.
     */
    virtual QString print() const = 0;
    virtual bool exec(TaffyDB*);

    /*!
     * List all files matching the query.
     *
     * @return  List of files matching the query.
     */
    QStringList getFiles() const;

    QStringList getResultSet() const;
    QStringList getErrors() const;
    QStringList getWarnings() const;

protected:
    void addResult(const QString &result);

    void postError(const QString &msg);
    void postWarning(const QString &msg);


private:
    struct Data;
    Data *d;
};

}

#endif // QUERY_H
