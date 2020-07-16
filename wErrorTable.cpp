#include "wErrorTable.h"
#include "ui_wErrorTable.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QDateTime>
#include <cStaffIDParserUtils.h>
#include <cKanbanParserUtils.h>
#include <QString>
#include <cConfigureUtils.h>
#include "cMessageBox.h"


wErrorTable::wErrorTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorTable)
{
    ui->setupUi(this);
    m_DataBase = cSQliteDatabase::instance();
    m_NumberPad = new cNumberPad();
    ui->vlkeypad->addWidget(m_NumberPad);
    m_CameraButton = new QPushButton();
    m_CameraButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(238, 238, 236); font-size: 30px; font-style: bold");
    m_CameraButton->setMinimumHeight(40);
    m_CameraButton->setText("CHỤP ẢNH");
    ui->vlkeypad->addWidget(m_CameraButton);
    connect(m_CameraButton, SIGNAL(clicked()), this, SIGNAL(sigCameraButtonClicked()));
    connect(m_NumberPad, SIGNAL(buttonClicked(QString)), this, SLOT(onButtonClicked(QString)));

}

wErrorTable::~wErrorTable()
{
    delete ui;
}

void wErrorTable::clearCheckedItems()
{
    for (int i = 0; i < m_TableRowWidgetList.count(); i++) {
        if (m_TableRowWidgetList.at(i)->isCheckBoxChecked()) {
            m_TableRowWidgetList.at(i)->clearCheckBoxChecked();
        }
    }
}

void wErrorTable::setMNV(QString text)
{
    m_MNV = text.toUpper();
    ui->lblMNV->setText(text.toUpper());
}

void wErrorTable::setMH(QString text)
{
    m_MH = text.toUpper();
    ui->lblMH->setText(text.toUpper());
}

void wErrorTable::createErrorTableContent(QList<QPair<QString, QString> > content)
{
    int row = 0, col = 0;
    int checkboxw, checkboxh;
    checkboxh = checkboxw = 40 - (content.count() - 10);
    if (checkboxh < 10 || checkboxw < 10) {
        checkboxh = checkboxw = 10;
    }

    qDebug() << "CheckBox Size: " << checkboxh << " " << checkboxw;

    int fontSize = 12 - (content.count() - 10);
    QFont ubuntuFont("Ubuntu", (fontSize > 5)?fontSize:5, QFont::Bold);
    qDebug() << "Text Font Size: " << ubuntuFont.pointSize();

    m_SignalMapper = new QSignalMapper();

    for (int i = 0; i < content.count(); i++) {
        QPair<QString, QString> rowElement = content.at(i);
        m_TableRowWidget = new cRowErrorWidget();
        m_TableRowWidget->setCheckBoxText(rowElement.second);
        m_TableRowWidget->setCheckBoxFont(ubuntuFont);
        m_TableRowWidget->setLineEditFont(ubuntuFont);
        m_TableRowWidget->setCheckBoxSize(QSize(checkboxw, checkboxh));
        m_TableRowWidgetList << m_TableRowWidget;
        m_SignalMapper->setMapping(m_TableRowWidgetList[i], i);
        connect( m_TableRowWidgetList[i], SIGNAL(clicked()), m_SignalMapper, SLOT(map()) );

        ui->glerrorContent->addWidget(m_TableRowWidgetList.last(), row++, col);
        if (row >= (content.count() / 2)) {
            row = 0;
            col += 1;
        }
    }
    connect( m_SignalMapper, SIGNAL(mapped(int)), this, SLOT(onCheckBoxChecked(qint32)));
    m_tableContent = content;
}

void wErrorTable::clearErrorTableContent()
{
    for (int i = 0; i < m_TableRowWidgetList.count(); i++) {
        if (m_SignalMapper !=nullptr)
            disconnect( m_TableRowWidgetList[i], SIGNAL(clicked()), m_SignalMapper, SLOT(map()) );
        ui->glerrorContent->removeWidget(m_TableRowWidgetList.at(i));
        delete m_TableRowWidgetList.at(i);
    }
    disconnect( m_SignalMapper, SIGNAL(mapped(int)), this, SLOT(onCheckBoxChecked(qint32)));
    m_TableRowWidgetList.clear();
    m_tableContent.clear();
    delete m_SignalMapper;
}

void wErrorTable::onCheckBoxChecked(qint32 index)
{
    m_CurrentcheckBox = index;
    qDebug() << "CheckBox checked at: " << m_CurrentcheckBox;
    qDebug() << "Ma Loi: " << m_tableContent.at(index).first << " Ten Loi: " <<  m_tableContent.at(index).second;
}

void wErrorTable::onButtonClicked(QString label)
{
    bool inValidDataFound = false;
    int SLL = 0;
    if(QString::compare(label, "del") == 0) {
        if (m_CurrentcheckBox >=0)
            m_TableRowWidgetList.at(m_CurrentcheckBox)->setLineEditText("");
    } else if (QString::compare(label, "enter")==0) {
        cDataSession dataSession;
        cDataSessionActivating tempDataSession = m_DataBase->getTempSession();
        dataSession.setHinh(tempDataSession.getPicturesList());
        dataSession.setmnv(cStaffIDParserUtils::getMNV(m_MNV));
        dataSession.settime(QDateTime::currentDateTime().toString("ddMMyyhhmmss"));
        dataSession.setseibango(cKanbanParserUtils::getSeibango(m_MH));
        dataSession.setsoto(cKanbanParserUtils::getSoTo(m_MH).toInt());
        dataSession.setsosoi(cKanbanParserUtils::getSoSoi(m_MH).toInt());
        dataSession.setblock(cKanbanParserUtils::getTenBlock(m_MH));
        dataSession.setca(cStaffIDParserUtils::getCa(m_MNV));
        dataSession.setdeviceid(cConfigureUtils::getWifiMac());
        QList<QPair<QString, int>> loi;
        int sosoi = cKanbanParserUtils::getSoSoi(m_MH).toInt();
        for (int i = 0; i < m_TableRowWidgetList.count(); i++) {
            if (m_TableRowWidgetList.at(i)->isCheckBoxChecked()) {
                bool isConvertOK = false;
                QString maLoi = m_tableContent.at(i).first;
                int tempSLL = m_TableRowWidgetList.at(i)->getLineEditText().toInt(&isConvertOK, 10);
                if (isConvertOK) {
                    if (tempSLL == 0 || tempSLL > sosoi) {
                        inValidDataFound = true;
                        break;
                    } else {
                        SLL += tempSLL;
                        if (SLL > sosoi)
                        {
                            inValidDataFound = true;
                            break;
                        }
                        loi.append(QPair<QString, int>(maLoi, tempSLL));
                    }
                } else {
                    inValidDataFound = true;
                }
            }
        }
        dataSession.setloi(loi);
        if (!inValidDataFound) {
            bool retVal = m_DataBase->insertHistoryTransaction(dataSession, cSQliteDatabase::UNSUBMITED);
            if (retVal)
                m_DataBase->deleteTempSession();
            emit finished();
            for (int i = 0; i < m_TableRowWidgetList.count(); i++) {
                if (m_TableRowWidgetList.at(i)->isCheckBoxChecked()) {
                    m_TableRowWidgetList.at(i)->clearCheckBoxChecked();
                }
            }
        }
        else if (inValidDataFound){
            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Tìm thấy số lượng lỗi không hợp lệ");
            m_MessageBox->setInformativeText("Số lượng lỗi phải khác 0 và nhỏ hơn hoặc bằng số sợi, vui lòng kiểm tra lại");
            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
            m_MessageBox->setHideRejectButton();
            m_MessageBox->exec();
            delete m_MessageBox;
        }
    } else {
        if (m_CurrentcheckBox >=0)
            m_TableRowWidgetList.at(m_CurrentcheckBox)->setLineEditText(label.toUpper());
    }
}
