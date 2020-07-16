#include "cSettingButton.h"
#include <QPixmap>

cSettingButton::cSettingButton(QWidget *parent)
{
    Q_UNUSED(parent);
    QPixmap mypix (":/images/resources/setting_blue.png");
    setPixmap(mypix);
}

cSettingButton::~cSettingButton()
{

}

void cSettingButton::mouseReleaseEvent(QMouseEvent *event)
{

    QPixmap mypix (":/images/resources/setting_blue.png");
    setPixmap(mypix);
    emit sigMouseReleaseEvent();
    QLabel::mouseReleaseEvent(event);
}

void cSettingButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (":/images/resources/setting_white.png");
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}
