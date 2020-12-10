#include "cConfirmNoButton.h"
#include <QPixmap>
#include <QDebug>

cConfirmNoButton::cConfirmNoButton(QWidget *parent)
{
    Q_UNUSED(parent);
    QPixmap mypix (":/images/resources/unchecked_60x60.png");
    setPixmap(mypix);
}

cConfirmNoButton::~cConfirmNoButton()
{

}

void cConfirmNoButton::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "cConfirmNoButton::mouseReleaseEvent-USer clicked here";
    QPixmap mypix (":/images/resources/unchecked_60x60.png");
    setPixmap(mypix);
    emit sigMouseReleaseEvent();
    QLabel::mouseReleaseEvent(event);
}

void cConfirmNoButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/unchecked_60x60_clear.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}
