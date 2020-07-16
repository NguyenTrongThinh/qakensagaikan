#include "cKanbanParserUtils.h"

cKanbanParserUtils::cKanbanParserUtils(QObject *parent) : QObject(parent)
{

}

cKanbanParserUtils::~cKanbanParserUtils()
{

}

QString cKanbanParserUtils::getSeibango(QString kanban)
{
    return kanban.left(5).toUpper();
}

QString cKanbanParserUtils::getSoSoi(QString kanban)
{
    return kanban.mid(5, 2).toUpper();
}

QString cKanbanParserUtils::getSoTo(QString kanban)
{
    return kanban.mid(7, 3).toUpper();
}

QString cKanbanParserUtils::getTenBlock(QString kanban)
{
    return kanban.toUpper().right(2);
}
