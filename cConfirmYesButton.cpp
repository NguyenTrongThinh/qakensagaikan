#include "cConfirmYesButton.h"
#include <QPixmap>

cConfirmYesButton::cConfirmYesButton(QWidget *parent)
{
    Q_UNUSED(parent);
    QPixmap mypix (":/images/resources/checked_60x60.png");
    setPixmap(mypix);
}

cConfirmYesButton::~cConfirmYesButton()
{

}

void cConfirmYesButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/checked_60x60.png");
    setPixmap(mypix);
    emit sigMouseReleaseEvent();
    QLabel::mouseReleaseEvent(event);
}

void cConfirmYesButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/checked_60x60_clear.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}
