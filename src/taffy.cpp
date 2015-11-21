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

namespace taffy {

enum TagAction
{
    NoAction,
    AddTagAction,
    RemoveTagAction,
    ShowTagsAction,
    ListFilesAction
};

struct TaggingQuery
{
    TaggingQuery() : tagAction(NoAction) { }
    bool setTagAction(TagAction ta) {
        if (tagAction != NoAction) {
            return false;
        }
        tagAction = ta;
        return true;
    }

    QStringList files;
    QString tag;
    TagAction tagAction;
};

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, TaggingQuery *query, QString *errorMessage)
{
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

    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        *errorMessage = parser.errorText();
        return CommandLineError;
    }

    if (parser.isSet(versionOption))
        return CommandLineVersionRequested;

    if (parser.isSet(helpOption))
        return CommandLineHelpRequested;

     const QStringList positionalArguments = parser.positionalArguments();

     if (parser.isSet(addTagOption)) {
         if (!query->setTagAction(AddTagAction)) {
             *errorMessage = "Can only execute one tagging action.";
             return CommandLineError;
         } else {
             if (positionalArguments.isEmpty()) {
                 *errorMessage = "You must specify at least one file.";
                 return CommandLineError;
             }
             query->tag = parser.value(addTagOption);
             query->files = parser.positionalArguments();
         }
     }

    if (parser.isSet(removeTagOption)) {
        if (!query->setTagAction(RemoveTagAction)) {
            *errorMessage = "Can only execute one tagging action.";
            return CommandLineError;
        } else {
            if (positionalArguments.isEmpty()) {
                *errorMessage = "You must specify at least one file.";
                return CommandLineError;
            }
            query->tag = parser.value(removeTagOption);
            query->files = parser.positionalArguments();
        }
    }

    if (parser.isSet(showTagsOption)) {
        if (!query->setTagAction(ShowTagsAction)) {
            *errorMessage = "Can only execute one tagging action.";
            return CommandLineError;
        } else {
            if (positionalArguments.isEmpty()) {
                *errorMessage = "You must specify at least one file.";
                return CommandLineError;
            }
            query->files = parser.positionalArguments();
        }
    }

    if (parser.isSet(listFilesOption)) {
        if (!query->setTagAction(ListFilesAction)) {
            *errorMessage = "Can only execute one tagging action.";
            return CommandLineError;
        } else {
            if (!positionalArguments.isEmpty()) {
                *errorMessage = QString("Unexpected argument(s): %1.").arg(positionalArguments.join(", "));
                return CommandLineError;
            }
            query->tag = parser.value(listFilesOption);
        }
    }

    if (query->tagAction == NoAction) {
        *errorMessage = "You must specify an action.";
        return CommandLineError;
    }

    return CommandLineOk;
}


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

    TaggingQuery query;
    QString errorMessage;

    switch (parseCommandLine(parser, &query, &errorMessage)) {
    case CommandLineOk:
        break;
    case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        return 0;
    case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

#ifdef QT_DEBUG
    qDebug() << "Query is:";
    switch (query.tagAction) {
    case AddTagAction:
        qDebug() << "Add tag" << query.tag << "on file(s)" << query.files.join(", ");
        break;
    case RemoveTagAction:
        qDebug() << "Remove tag" << query.tag << "on file(s)" << query.files.join(", ");
        break;
    case ShowTagsAction:
        qDebug() << "Show tags of file(s)" << query.files.join(", ");
        break;
    case ListFilesAction:
        qDebug() << "List files with tag" << query.tag;
        break;
    case NoAction:
        qDebug() << "Ooops... something went wrong here.";
    }
#endif
    return 0;
}

}
