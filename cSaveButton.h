#ifndef CSAVEBUTTON_H
#define CSAVEBUTTON_H
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>


class cSaveButton : public QLabel
{
    Q_OBJECT
public:
    explicit cSaveButton(QWidget *parent = nullptr);
    ~cSaveButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CSAVEBUTTON_H
