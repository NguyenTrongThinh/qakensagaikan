#include "wSelectErrorTable.h"
#include "ui_wSelectErrorTable.h"
#include <QDebug>

wSelectErrorTable::wSelectErrorTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectErrorTable)
{
    ui->setupUi(this);
}

wSelectErrorTable::~wSelectErrorTable()
{
    delete ui;
}

void wSelectErrorTable::setErrorTableList(QStringList list)
{
    m_ErrorTableList = list;
}

void wSelectErrorTable::createButtonTable()
{
    m_SigMapper = new QSignalMapper();
    foreach(QString lable, m_ErrorTableList) {
        m_PushButton = new QPushButton();
        m_PushButton->setText(lable.toUpper());
        m_PushButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(238, 238, 236); font-size: 30px; font-style: bold;");
        m_PushButton->setMinimumHeight(100);
        m_ListButon << m_PushButton;

        m_SigMapper->setMapping(m_ListButon[m_ErrorTableList.indexOf(lable)], m_ErrorTableList.indexOf(lable));
        connect( m_ListButon[m_ErrorTableList.indexOf(lable)], SIGNAL(clicked()), m_SigMapper, SLOT(map()) );

        ui->hlButton->addWidget(m_ListButon.last());
    }
    connect( m_SigMapper, SIGNAL(mapped(int)), this, SLOT(onButtonClick(qint32)));
}

QString wSelectErrorTable::getSelectedTableName()
{
    return m_SelectedTableName;
}

void wSelectErrorTable::onButtonClick(qint32 index)
{
    m_SelectedTableName = m_ErrorTableList.at(index);
    emit sigErrorTableSetected(m_SelectedTableName);
}
