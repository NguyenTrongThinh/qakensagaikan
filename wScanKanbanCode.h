#ifndef SCANKANBANCODE_H
#define SCANKANBANCODE_H

#include <QWidget>
#include <cScannerUtils.h>


namespace Ui {
class ScanKanbanCode;
}

class wScanKanbanCode : public QWidget
{
    Q_OBJECT

public:
    explicit wScanKanbanCode(QWidget *parent = nullptr);
    ~wScanKanbanCode();
    void setKanbanLable(QString kanbancode);
private:
    cScannerUtils *m_ScanerUtils;
    QString m_KanbanCode;
    Ui::ScanKanbanCode *ui;
signals:
    void sigScanKanbanDetected(const QString &id);
private slots:
};

#endif // SCANKANBANCODE_H
