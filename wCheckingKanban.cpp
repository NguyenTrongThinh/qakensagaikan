#include "wCheckingKanban.h"
#include "ui_wCheckingKanban.h"
#include "cConfigureUtils.h"
#include <QDebug>
#include <QLabel>

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

void wCheckingKanban::setMAB1(QString maab1)
{
    ui->lbAB1->setText(maab1);
}

void wCheckingKanban::setMAB2(QString maab2)
{
    ui->lbAB2->setText(maab2);
}

void wCheckingKanban::setLine(QString line)
{
    ui->lbLIne->setText(line);
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
    qDebug() << "wCheckingKanban::cancelOperation-Cancel Operation by User";
}

void wCheckingKanban::createMCUActionBox(int numBox)
{
    int row = 0, col = 0;
    QLayoutItem *child;
    qDebug() << "wCheckingKanban::createMCUActionBox-Start Delete child count" << ui->gridLayout_6->count();
    while ((child = ui->gridLayout_6->takeAt(0)) != 0) {
        qDebug() << "wCheckingKanban::createMCUActionBox-Delete child";
        delete child->widget();
        delete child;
    }
    if(numBox > 0)
    {
        for(int i = 0; i < numBox; i++)
        {
            QLabel *Label = new QLabel();
            Label->setStyleSheet("font-size: 15pt;");
            Label->setAlignment(Qt::AlignCenter);
            Label->setStyleSheet("QLabel{ width:50px; height: 40px; border-color: rgb(46, 52, 54);border: 1px solid black;}");
            Label->setText("OP" + QString::number(i + 1));
            ui->gridLayout_6->addWidget(Label, row, col++);
        }
    }

}

void wCheckingKanban::setOperatorStatus(int numOfOperator, bool isMCUConnect)
{
    if((ui->gridLayout_6->count() >= (numOfOperator + 1)) && (ui->gridLayout_6->count() > 0))
    {
        QLabel *myLabel = qobject_cast<QLabel*>(ui->gridLayout_6->itemAt(numOfOperator)->widget());
        myLabel->setStyleSheet("QLabel{ width:50px; height: 40px; border-color: rgb(46, 52, 54);background-color: rgb(138, 226, 52);border: 1px solid black;}");
    }
    if(ui->gridLayout_6->count() == (numOfOperator + 1))
    {
        if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
            if ((m_Timer != nullptr) && (m_Timer->isActive() == false))
            {
                m_Delay = cConfigureUtils::getDelayValue();
                m_Timer->start();
            }
        }
    }
}

void wCheckingKanban::onTimerTimeout()
{
    if (m_Delay > 0) {
        m_Delay--;
       // ui->btnOK->setText(QString::number(m_Delay).toUpper());

    } else {
        m_Timer->stop();
        //ui->btnOK->setText("OK");
        QLayoutItem *child;
        qDebug() << "Start Delete child count" << ui->gridLayout_6->count();
        while ((child = ui->gridLayout_6->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
        emit sigOKCkicked();
    }
}

void wCheckingKanban::onBtnOKClicked()
{
    if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
//        if (m_Timer != nullptr)
//        {
//            m_Delay = cConfigureUtils::getDelayValue();
//            m_Timer->start();
//        }
    } else {
        QLayoutItem *child;
        while ((child = ui->gridLayout_6->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
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


