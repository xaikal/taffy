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


#include "showtagsquery.h"

#include "model/taffydb.h"

namespace taffy {

ShowTagsQuery::ShowTagsQuery(const QStringList &files)
    : Query(files)
{

}

ShowTagsQuery::~ShowTagsQuery()
{

}

QString ShowTagsQuery::print() const
{
    return QString("Show tags for file(s) '%1'").arg(getFiles().join(", "));
}

bool ShowTagsQuery::exec(TaffyDB *db)
{
    QStringList files = this->getFiles();
    for (auto i = files.cbegin(); i != files.cend(); i++) {
        File f(*i);
        if (!f.exists()) {
            QString msg("File '%1' does not exist.");
            msg = msg.arg(*i);
            postError(msg);
            return false;
        }
        QList<std::shared_ptr<Tag> > tags = db->showTagsOfFile(f);
        for (auto j = tags.cbegin(); j != tags.cend(); j++) {
            std::shared_ptr<Tag> t = *j;
            addResult(t->getTag());
        }
    }
    return true;
}

}
