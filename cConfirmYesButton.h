#ifndef CCONFIRMYESBUTTON_H
#define CCONFIRMYESBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>

class cConfirmYesButton : public QLabel
{
    Q_OBJECT
public:
    cConfirmYesButton(QWidget *parent = nullptr);
    ~cConfirmYesButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CCONFIRMYESBUTTON_H
