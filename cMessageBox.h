#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H

#include <QDialog>
#include <QObject>

namespace Ui {
class cMessageBox;
}

class cMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit cMessageBox(QWidget *parent = 0);
    ~cMessageBox();
    void setText(QString text);
    void setInformativeText(QString text);
    void setIcon(QPixmap pixmap);
    void setHideRejectButton();
protected:
    void changeEvent(QEvent *event);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::cMessageBox *ui;
};

#endif // CMESSAGEBOX_H
