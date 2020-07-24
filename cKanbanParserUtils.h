#ifndef CKANBANPARSERUTILS_H
#define CKANBANPARSERUTILS_H

#include <QObject>

class cKanbanParserUtils : public QObject
{
    Q_OBJECT
public:
    explicit cKanbanParserUtils(QObject *parent = nullptr);
    ~cKanbanParserUtils();
    static QString getMH(QString kanban);
    static QString getDatePrint(QString kanban);
    static QString getMHNamePlate(QString kanban);


signals:

public slots:
};

#endif // CKANBANPARSERUTILS_H
