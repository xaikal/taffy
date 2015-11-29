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


#ifndef LISTFILESQUERY_H
#define LISTFILESQUERY_H

#include "query.h"

/*!
 * A query that lists files with a certain tag.
 */
class ListFilesQuery : public Query
{
public:
    /*!
     * Creates a new ListFilesQuery.
     *
     * @param   tag     The tag that is searched for.
     * @param   files   An optional pattern of files to restrict the search.
     */
    ListFilesQuery(const QString &tag, const QStringList &files);
    virtual ~ListFilesQuery();

    /*!
     * Returns the tag that is searched for.
     *
     * @return  Tag that is searched for
     */
    QString getTag() const;

    virtual QString print() const override;

private:
    QString tag;
};

#endif // LISTFILESQUERY_H
