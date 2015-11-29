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


#include "tag.h"

namespace taffy {

struct Tag::Data {
    int identifier;
    QString tag;
};

Tag::Tag(const QString &tag, int id)
 : DbObject(id), d(new Data)
{
    d->identifier = -1;
    d->tag = tag;
}

Tag::~Tag()
{
    delete d;
}

QString Tag::getTag() const
{
    return d->tag;
}

}
