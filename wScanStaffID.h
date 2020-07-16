#ifndef SCANSTAFFID_H
#define SCANSTAFFID_H

#include <QWidget>
#include <cScannerUtils.h>

namespace Ui {
class ScanStaffID;
}

class wScanStaffID : public QWidget
{
    Q_OBJECT

public:
    explicit wScanStaffID(QWidget *parent = nullptr);
    ~wScanStaffID();
    void setStaffIDLable(QString text);

private:
    Ui::ScanStaffID *ui;
    cScannerUtils *m_ScanerUtils;
    QString m_StaffID;
signals:
    void sigStaffIDDetected(const QString &id);
private slots:
};

#endif // SCANSTAFFID_H
