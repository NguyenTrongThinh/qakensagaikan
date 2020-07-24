#ifndef CCUSTOMLINEEDIT_H
#define CCUSTOMLINEEDIT_H

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QFocusEvent>

class cCustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit cCustomLineEdit(QWidget *parent = nullptr);
    ~cCustomLineEdit();
    void mouseReleaseEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *evt);
signals:
    void sigFocusInEvent();
    void sigFocusOutEvent();
    void sigMouseReleaseEvent();
    void sigMousePressEvent();
};

#endif // CCUSTOMLINEEDIT_H
