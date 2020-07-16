#include "cCancelButton.h"
#include <QPixmap>

cCancelButton::cCancelButton(QWidget *parent)
{
    Q_UNUSED(parent);
    m_IconNornal = QString(":/images/resources/human_pink.png");
    m_IconClicked = QString(":/images/resources/human_gray.png");
    QPixmap mypix (m_IconNornal);
    setPixmap(mypix);
}

cCancelButton::~cCancelButton()
{

}

void cCancelButton::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QPixmap mypix (m_IconNornal);
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}

void cCancelButton::mousePressEvent(QMouseEvent *event)
{
    QPixmap mypix (m_IconClicked);
    setPixmap(mypix);
    QLabel::mouseReleaseEvent(event);
}

void cCancelButton::setICon(QString normal, QString clicked)
{
    m_IconNornal = normal;
    m_IconClicked = clicked;
    QPixmap mypix (m_IconNornal);
    setPixmap(mypix);
}
