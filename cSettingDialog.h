#ifndef CSETTINGDIALOG_H
#define CSETTINGDIALOG_H

#include <QDialog>
#include "cNumberPad2.h"

namespace Ui {
class cSettingDialog;
}

class cSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cSettingDialog(QWidget *parent = 0);
    ~cSettingDialog();
private:
    cNumberPad2 *m_NumberPad = nullptr;

private:
    void setLineEditText(const QString &text);
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

    void onKeypadClicked(QString val);
private:
    Ui::cSettingDialog *ui;
};

#endif // CSETTINGDIALOG_H
