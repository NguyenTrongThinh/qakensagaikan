#ifndef CTRASHBUTTON_H
#define CTRASHBUTTON_H
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>


class cTrashButton : public QLabel
{
    Q_OBJECT
public:
    explicit cTrashButton(QWidget *parent = nullptr);
    ~cTrashButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CTRASHBUTTON_H
