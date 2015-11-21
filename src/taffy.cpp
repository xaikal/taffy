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


#include "taffy.h"

#include <QCommandLineParser>

#include <QtDebug>

using namespace taffy;

Taffy::Taffy(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    setApplicationName("Taffy");
    setApplicationVersion("0.0.1");
}

int Taffy::run()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Taffy: A file tagging utility.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("[files]",
                                 QCoreApplication::translate("main", "Files to change tags for (where applicable)."));

    QCommandLineOption addTagOption(QStringList() << "a" << "add",
                                    QCoreApplication::translate("main", "Add tag <tag>."),
                                    QCoreApplication::translate("main", "tag"));
    parser.addOption(addTagOption);

    QCommandLineOption removeTagOption(QStringList() << "r" << "remove",
                                       QCoreApplication::translate("main", "Remove tag <tag>."),
                                       QCoreApplication::translate("main", "tag"));
    parser.addOption(removeTagOption);

    QCommandLineOption showTagsOption(QStringList() << "s" << "show",
                                      QCoreApplication::translate("main", "Show tags."));
    parser.addOption(showTagsOption);

    QCommandLineOption listFilesOption(QStringList() << "l" << "list",
                                       QCoreApplication::translate("main", "List files tagged with <tag>."),
                                       QCoreApplication::translate("main", "tag"));
    parser.addOption(listFilesOption);

    // Process the actual command line arguments given by the user
    parser.process(*this);

#ifdef QT_DEBUG
    qDebug() << "Positional arguments: " << parser.positionalArguments() << endl;
#endif

    return 0;
}

