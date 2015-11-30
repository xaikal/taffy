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


#ifndef TAFFYDB_H
#define TAFFYDB_H

#include "model.data/file.h"
#include "model.data/tag.h"

#include <QList>

#include <memory>

namespace taffy {

class TaffyDB
{
public:
    TaffyDB();
    ~TaffyDB();

    bool connect();
    bool disconnect();

    void addTagToFile(Tag &tag, File &file);
    void removeTagFromFile(Tag &tag, File &file);
    QList<std::shared_ptr<Tag> > showTagsOfFile(File &file);
    QList<std::shared_ptr<File> > listFilesWithTag(Tag &tag);

    QString getDBLocation();
    void setDBLocation(const QString &path);

private:
    struct Data;
    Data *d;
};

}

#endif // TAFFYDB_H
