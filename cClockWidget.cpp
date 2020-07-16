#include "cClockWidget.h"
#include <QDateTime>

cClockWidget::cClockWidget(QWidget *parent) : QWidget(parent)
{
    QFont ubuntuTimeFont("Ubuntu", 15, QFont::Bold);
    QFont ubuntuDateFont("Ubuntu", 12, QFont::Bold);
    m_Timer = new QTimer();
    m_Timer->setInterval(500);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTicked()));
    m_VBoxLayout = new QVBoxLayout();
    m_Time = new QLabel();
    m_Date = new QLabel();
    m_Time->setFont(ubuntuTimeFont);
    m_Time->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    m_Date->setFont(ubuntuDateFont);
    m_Date->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    m_VBoxLayout->addWidget(m_Time, Qt::AlignCenter);
    m_VBoxLayout->addWidget(m_Date, Qt::AlignCenter);
    m_VBoxLayout->setSpacing(0);
    m_VBoxLayout->setMargin(0);
    this->setLayout(m_VBoxLayout);
    m_Timer->start();
}

cClockWidget::~cClockWidget()
{
    m_Timer->stop();
    m_VBoxLayout->removeWidget(m_Time);
    m_VBoxLayout->removeWidget(m_Date);
    delete m_Timer;
    delete m_Time;
    delete m_Date;
    delete m_VBoxLayout;
}

void cClockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit onClockWidgetClicked();
    QWidget::mouseReleaseEvent(event);
}

void cClockWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void cClockWidget::onTimerTicked()
{
    static QDateTime current;
    current = QDateTime::currentDateTime();
    if (m_Date != nullptr) {
        m_Date->setText(current.toString("dd-MM-yy"));
    }
    if (m_Time != nullptr) {
        m_Time->setText(current.toString("hh:mm"));
    }
}

