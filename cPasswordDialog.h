#ifndef CPASSWORDDIALOG_H
#define CPASSWORDDIALOG_H

#include <QDialog>
#include "cNumberPad2.h"

namespace Ui {
class cPasswordDialog;
}

class cPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cPasswordDialog(QWidget *parent = 0);
    ~cPasswordDialog();
    QString getPassword();
private:
    void setLineEditText(const QString &text);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    cNumberPad2 *m_NumberPad = nullptr;
private:
    Ui::cPasswordDialog *ui;
private slots:
    void onKeypadClicked(QString val);
};

#endif // CPASSWORDDIALOG_H
