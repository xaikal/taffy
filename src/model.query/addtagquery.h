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


#ifndef ADDTAGQUERY_H
#define ADDTAGQUERY_H

#include "query.h"

/*!
 * A query that adds a tag to one or more files.
 */
class AddTagQuery : public Query
{
public:
    /*!
     * Creates a new AddTagQuery.
     *
     * @param   tag     The tag that is added to the file(s).
     * @param   files   A list of files that are tagged.
     */
    AddTagQuery(const QString &tag, const QStringList &files);
    virtual ~AddTagQuery();

    /*!
     * Returns the tag that is added.
     *
     * @return  Tag that is added
     */
    QString getTag() const;

    virtual QString print() const override;

private:
    QString tag;
};

#endif // ADDTAGQUERY_H
