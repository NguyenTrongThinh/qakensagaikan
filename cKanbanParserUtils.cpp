#include "cKanbanParserUtils.h"

cKanbanParserUtils::cKanbanParserUtils(QObject *parent) : QObject(parent)
{

}

cKanbanParserUtils::~cKanbanParserUtils()
{

}

QString cKanbanParserUtils::getMH(QString kanban)
{
    return kanban.left(10).toUpper();
}

QString cKanbanParserUtils::getDatePrint(QString kanban)
{
    return kanban.mid(13, 6).toUpper();
}

QString cKanbanParserUtils::getMHNamePlate(QString kanban)
{
    return kanban.mid(19, 4).toUpper();
}


