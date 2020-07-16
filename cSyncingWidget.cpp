#include "cSyncingWidget.h"
#include <QPixmap>



cSyncingWidget::cSyncingWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    m_Movie = new QMovie();
    setSyncDoneState();
}

cSyncingWidget::~cSyncingWidget()
{
    m_Movie->stop();
    delete m_Movie;
}

void cSyncingWidget::setSyncingState()
{
    if (m_Movie != nullptr) {
        m_Movie->stop();
        m_Movie->setFileName(":/images/resources/syncing_60x60.gif");
        setMovie(m_Movie);
        m_Movie->start();
    }
}

void cSyncingWidget::setSyncDoneState()
{
    if (m_Movie != nullptr) {
        m_Movie->stop();
        QPixmap mypix (":/images/resources/done.png");
        setPixmap(mypix);
    }
}

void cSyncingWidget::setSyncDoneStateWithWarning()
{
    if (m_Movie != nullptr) {
        m_Movie->stop();
        QPixmap mypix (":/images/resources/done_withwarn.png");
        setPixmap(mypix);
    }
}
