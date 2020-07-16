#include "wCheckingKanban.h"
#include "ui_wCheckingKanban.h"
#include "cConfigureUtils.h"
#include <QDebug>
wCheckingKanban::wCheckingKanban(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wCheckingKanban)
{
    ui->setupUi(this);
    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onBtnOKClicked()));
    connect(ui->btnNotGood, SIGNAL(clicked()), this, SLOT(onbtnNGClicked()));
    connect(ui->btnSetting, SIGNAL(sigMouseReleaseEvent()), this, SLOT(onbtnSettingClicked()));
    m_Delay = cConfigureUtils::getDelayValue();
    m_Timer = new QTimer();
    m_Timer->setInterval(1000);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));

}

wCheckingKanban::~wCheckingKanban()
{
    m_Timer->stop();
    delete m_Timer;
    delete ui;
}

void wCheckingKanban::setMNV(QString mnv)
{
    ui->lblMNV->setText(mnv);
}

void wCheckingKanban::setMKB(QString kanban)
{
    ui->lblMH->setText(kanban);
}

void wCheckingKanban::setDelayTiming(int sec)
{
    m_Delay = sec;
}

void wCheckingKanban::cancelOperation()
{
    if (m_Timer != nullptr) {
        m_Timer->stop();
    }
    qDebug() << "Cancel Operation by User";
}

void wCheckingKanban::onTimerTimeout()
{
    if (m_Delay > 0) {
        m_Delay--;
       // ui->btnOK->setText(QString::number(m_Delay).toUpper());

    } else {
        m_Timer->stop();
        //ui->btnOK->setText("OK");
        emit sigOKCkicked();
    }
}

void wCheckingKanban::onBtnOKClicked()
{
    if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
        if (m_Timer != nullptr)
        {
            m_Delay = cConfigureUtils::getDelayValue();
            //ui->btnOK->setText(QString::number(m_Delay).toUpper());
            m_Timer->start();
        }
    } else {
        emit sigOKCkicked();
    }
}

void wCheckingKanban::onbtnNGClicked()
{
    if (m_Timer != nullptr) {
        m_Timer->stop();
    }
    //ui->btnOK->setText("OK");
    emit sigNGClicked();
}

void wCheckingKanban::onbtnSettingClicked()
{
    if (m_Timer != nullptr) {
        m_Timer->stop();
    }
    emit sigSettingClicked();

}


