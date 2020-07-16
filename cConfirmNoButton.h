#ifndef CCONFIRMNOBUTTON_H
#define CCONFIRMNOBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>

class cConfirmNoButton : public QLabel
{
    Q_OBJECT
public:
    cConfirmNoButton(QWidget *parent = nullptr);
    ~cConfirmNoButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CCONFIRMNOBUTTON_H
