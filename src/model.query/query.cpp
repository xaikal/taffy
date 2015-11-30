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


#include "query.h"

namespace taffy {

struct Query::Data {
    QStringList files;

    QStringList results;
    QStringList errors;
    QStringList warnings;

    Data(const QStringList &files) : files(files) { }
};

Query::Query(const QStringList &files)
  : d(new Data(files))
{

}

Query::~Query()
{
    delete d;
}

bool Query::exec(TaffyDB*)
{
    return false;
}

QStringList Query::getFiles() const
{
    return d->files;
}

QStringList Query::getResultSet() const
{
    return d->results;
}

QStringList Query::getErrors() const
{
    return d->errors;
}

QStringList Query::getWarnings() const
{
    return d->warnings;
}

void Query::addResult(const QString &result)
{
    d->results.append(result);
}

void Query::postError(const QString &msg)
{
    d->errors.append(msg);
}

void Query::postWarning(const QString &msg)
{
    d->warnings.append(msg);
}

}
