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
    void setMaAB1(QString maab1);
    void setMaAB2(QString maab2);
private:
    cScannerUtils *m_ScanerUtils;
    QString m_KanbanCode;
    QString m_AB1Code;
    QString m_AB2Code;
    Ui::ScanKanbanCode *ui;
signals:
    void sigScanKanbanDetected(const QString &id);
private slots:
};

#endif // SCANKANBANCODE_H
