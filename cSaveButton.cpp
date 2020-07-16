#include "cSaveButton.h"
#include <QPixmap>

cSaveButton::cSaveButton(QWidget *parent)
{
    Q_UNUSED(parent);
    QPixmap mypix (":/images/resources/save.png");
    setPixmap(mypix);
}

cSaveButton::~cSaveButton()
{

}

void cSaveButton::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QPixmap mypix (":/images/resources/save.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}

void cSaveButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/save_clicked.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}
