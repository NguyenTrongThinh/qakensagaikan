#ifndef WSELECTLINE_H
#define WSELECTLINE_H

#include <QDialog>
#include "cNumberPad2.h"
#include <QLineEdit>

namespace Ui {
class wSelectLine;
}

class wSelectLine : public QDialog
{
    Q_OBJECT

public:
    explicit wSelectLine(QWidget *parent = 0);
    ~wSelectLine();
private:
    cNumberPad2 *m_NumberPad = nullptr;

private:
    void setLineEditText(const QString &text);
    void setInputObject(QLineEdit *objLineEdit);
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

    void onKeypadClicked(QString val);

public slots:

private:
    Ui::wSelectLine *ui;
};


#endif // WSELECTLINE_H
