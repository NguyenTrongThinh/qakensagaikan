#ifndef CKANBANPARSERUTILS_H
#define CKANBANPARSERUTILS_H

#include <QObject>

class cKanbanParserUtils : public QObject
{
    Q_OBJECT
public:
    explicit cKanbanParserUtils(QObject *parent = nullptr);
    ~cKanbanParserUtils();
    static QString getSeibango(QString kanban);
    static QString getSoSoi(QString kanban);
    static QString getSoTo(QString kanban);
    static QString getTenBlock(QString kanban);

signals:

public slots:
};

#endif // CKANBANPARSERUTILS_H
