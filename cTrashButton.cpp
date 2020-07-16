#include "cTrashButton.h"
#include <QPixmap>

cTrashButton::cTrashButton(QWidget *parent)
{
    Q_UNUSED(parent);
    QPixmap mypix (":/images/resources/trash.png");
    setPixmap(mypix);
}

cTrashButton::~cTrashButton()
{

}

void cTrashButton::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QPixmap mypix (":/images/resources/trash.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}

void cTrashButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/trash_clicked.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}
