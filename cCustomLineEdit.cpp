#include "cCustomLineEdit.h"



cCustomLineEdit::cCustomLineEdit(QWidget *parent)
{
    Q_UNUSED(parent);
    setReadOnly(false);
}

cCustomLineEdit::~cCustomLineEdit()
{

}

void cCustomLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QLineEdit::mouseReleaseEvent(event);
}

void cCustomLineEdit::focusOutEvent(QFocusEvent *event)
{
    emit sigFocusOutEvent();
    QLineEdit::focusOutEvent(event);
}

void cCustomLineEdit::focusInEvent(QFocusEvent *event)
{
    emit sigFocusInEvent();
    QLineEdit::focusInEvent(event);
}

void cCustomLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    setReadOnly(false);
    QLineEdit::keyPressEvent(event);
//    setReadOnly(true);
}

void cCustomLineEdit::mousePressEvent(QMouseEvent *evt)
{
    emit sigMousePressEvent();
    QLineEdit::mousePressEvent(evt);
}
