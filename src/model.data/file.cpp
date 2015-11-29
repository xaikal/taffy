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


#include "file.h"

#include <QFileInfo>

struct File::Data {
    QFileInfo file;
};

File::File(const QString &fileName, int id)
  : DbObject(id), d(new Data)
{
    d->file.setFile(fileName);
}

File::~File()
{
    delete d;
}

bool File::exists() const
{
    return d->file.exists();
}

QString File::getPath() const
{
    return d->file.absoluteFilePath();
}

int File::getSize() const
{
    return d->file.size();
}

QString File::getHash() const
{
    return QString();
}

QDateTime File::getLastModified() const
{
    return d->file.lastModified();
}

