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


#include "listfilesquery.h"

ListFilesQuery::ListFilesQuery(const QString &tag, const QStringList &files)
    : Query(files), tag(tag)
{

}

ListFilesQuery::~ListFilesQuery()
{

}

QString ListFilesQuery::getTag() const
{
    return tag;
}

QString ListFilesQuery::print() const
{
    QStringList files(getFiles());
    if (!files.isEmpty()) {
        return QString("Show files with tag '%1' matching '%2'").arg(getTag()).arg(files.join(", "));
    }
    return QString("Show files with tag '%1'").arg(getTag());
}
