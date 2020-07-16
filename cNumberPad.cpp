#include "cNumberPad.h"
#include <QDebug>

cNumberPad::cNumberPad(QWidget *parent) : QWidget(parent)
{
    m_ButtonLabel << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0" << "del" << "enter";
    int row = 0, col = 0;
    m_signalMapperKey = new QSignalMapper();
    m_GridLayout = new QGridLayout();
    m_GridLayout->setMargin(0);
    for(int i = 0; i < m_ButtonLabel.count(); i++) {
        m_PushButton = new QPushButton();
        m_PushButton->setMinimumHeight(40);
        m_PushButton->setText(m_ButtonLabel.at(i).toUpper());
        m_PushButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(238, 238, 236); pressed{ background-color: rgb(20, 101, 164);}; font-size: 30px; font-style: bold");
        m_ListButton << m_PushButton;
        m_signalMapperKey->setMapping(m_ListButton.at(i), i);
        connect( m_ListButton.at(i), SIGNAL(clicked()), m_signalMapperKey, SLOT(map()) );
        if (QString::compare(m_ButtonLabel.at(i), "del") && QString::compare(m_ButtonLabel.at(i), "enter")) {
            m_GridLayout->addWidget(m_ListButton.last(), row, col++);
            if (col>2) {
                row++;
                col = 0;
            }
        }
        else {
            if (QString::compare(m_ButtonLabel.at(i), "del") == 0)
            {
                m_PushButton->setStyleSheet("background-color: rgb(0, 255, 255); color: rgb(238, 238, 236); font-size: 30px; font-style: bold");

                m_GridLayout->addWidget(m_ListButton.at(i), row, col++, 1, 2);
                row++;
                col = 0;
            }
            else if (QString::compare(m_ButtonLabel.at(i), "enter") == 0)  {
                m_PushButton->setStyleSheet("background-color: rgb(184, 116, 26); color: rgb(238, 238, 236); font-size: 30px; font-style: bold");
                m_GridLayout->addWidget(m_ListButton.at(i), row, col++, 1, 3);
            }
        }
    }

    connect( m_signalMapperKey, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(qint32)));
    this->setLayout(m_GridLayout);
}

cNumberPad::~cNumberPad()
{
    for (int i = 0; i < m_ListButton.count(); i++) {
        if (m_signalMapperKey != nullptr) {
            disconnect( m_ListButton.at(i), SIGNAL(clicked()), m_signalMapperKey, SLOT(map()) );
        }
        m_GridLayout->removeWidget(m_ListButton.at(i));
        delete m_ListButton.at(i);
    }
    disconnect( m_signalMapperKey, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(qint32)));
    m_ListButton.clear();
    m_ButtonLabel.clear();
    delete m_signalMapperKey;
}

void cNumberPad::onButtonClicked(qint32 index)
{
    qDebug() << "Button Clicked at: " << index;
    emit buttonClicked(m_ButtonLabel.at(index));
}
