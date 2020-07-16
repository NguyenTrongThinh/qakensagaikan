#include "cWifiWidget.h"
#include <QPixmap>

cWifiWidget::cWifiWidget(QWidget *parent) : QWidget(parent)
{
    m_Process = new QProcess();
    m_Timer = new QTimer();
    m_Timer->setInterval(1000);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTicked()));
    m_HBoxLayout = new QHBoxLayout();
    m_IconLabel = new QLabel();
    m_HBoxLayout->addWidget(m_IconLabel);
    m_HBoxLayout->setSpacing(0);
    m_HBoxLayout->setMargin(0);
    this->setLayout(m_HBoxLayout);
    m_Timer->start();
    setIconDisconnected();
}

cWifiWidget::~cWifiWidget()
{
    m_Timer->stop();
    m_HBoxLayout->removeWidget(m_IconLabel);
    delete m_Timer;
    delete m_Process;
    delete m_IconLabel;
    delete m_HBoxLayout;
}

void cWifiWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigWifiWidgetClicked();
    QWidget::mouseReleaseEvent(event);
}

void cWifiWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

QString cWifiWidget::getWifiBSSID()
{
    QString program = "iwgetid";
    QString output = "";
    if (m_Process != nullptr) {
        m_Process->start(program);
        m_Process->waitForFinished(500);
        output = m_Process->readAllStandardOutput();
        if (output.contains("ESSID")) {
            output = output.split(':').last();
            output.remove('"');
        } else {
            output = "";
        }
    }
    return output;
}

void cWifiWidget::setIconDisconnected()
{
    if (m_IconLabel != nullptr) {
        QPixmap mypix (":/images/resources/internet_lost.png");
        m_IconLabel->setPixmap(mypix);
    }
}

void cWifiWidget::setIconConnected()
{
    if (m_IconLabel != nullptr) {
        QPixmap mypix (":/images/resources/internet_have.png");
        m_IconLabel->setPixmap(mypix);
    }
}

void cWifiWidget::onTimerTicked()
{
    if (getWifiBSSID() != "") {
        if (!m_isWifiConnected) {
            emit sigWifiConnected();
            setIconConnected();
            m_isWifiConnected = true;
        }
    } else {
        if (m_isWifiConnected) {
            emit sigWifiDisconnected();
            setIconDisconnected();
            m_isWifiConnected = false;
        }
    }
}
