#include "cMCUWidget.h"
#include <QPixmap>

cMCUWidget::cMCUWidget(QWidget *parent) : QWidget(parent)
{
    m_Process = new QProcess();

    m_HBoxLayout = new QHBoxLayout();
    m_IconLabel = new QLabel();
    m_HBoxLayout->addWidget(m_IconLabel);
    m_HBoxLayout->setSpacing(0);
    m_HBoxLayout->setMargin(0);
    this->setLayout(m_HBoxLayout);
    setIconDisconnected();
}

cMCUWidget::~cMCUWidget()
{
    m_HBoxLayout->removeWidget(m_IconLabel);
    delete m_Process;
    delete m_IconLabel;
    delete m_HBoxLayout;
}


void cMCUWidget::setIconDisconnected()
{
    if (m_IconLabel != nullptr) {
        QPixmap mypix (":/images/resources/disconnect-usb.png");
        m_IconLabel->setPixmap(mypix);
    }
}

void cMCUWidget::setIconConnected()
{
    if (m_IconLabel != nullptr) {
        QPixmap mypix (":/images/resources/pendrive.png");
        m_IconLabel->setPixmap(mypix);
    }
}

