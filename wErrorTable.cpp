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
#include <QScroller>
#include <QScrollBar>
#include "cAB1CodeParserUtils.h"
#include "cAB2CodeParserUtils.h"

wErrorTable::wErrorTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorTable)
{
    ui->setupUi(this);
    m_DataBase = cSQliteDatabase::instance();
    m_NumberPad = new cNumberPad();
    ui->vlkeypad->addWidget(m_NumberPad);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    m_CameraButton = new QPushButton();
    m_CameraButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(238, 238, 236); font-size: 30px; font-style: bold");
    m_CameraButton->setMinimumHeight(40);
    m_CameraButton->setText("CHỤP ẢNH");
    ui->vlkeypad->addWidget(m_CameraButton);
    ui->scrollArea->verticalScrollBar()->setSingleStep(20);
    ui->scrollArea->verticalScrollBar()->setPageStep(60);
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

void wErrorTable::setAB1(QString ab1)
{
    if(ab1.isEmpty() == false)
    {
        m_AB1 = ab1;
        ui->lbAB1->setText(ab1.toUpper());
    }
    else
    {
        m_AB1 = "";
    }

}

void wErrorTable::setAB2(QString ab2)
{
    if(ab2.isEmpty() == false)
    {
        m_AB2 = ab2;
        ui->lbAB2->setText(ab2.toUpper());
    }
    else
    {
        m_AB2 = "";
    }
}

void wErrorTable::createMCUActionBox(int numOfBox)
{

}

void wErrorTable::createErrorTableContent(QList<QPair<QString, QString> > content)
{
    int row = 0, col = 0;
    int checkboxw, checkboxh;
//    checkboxh = checkboxw = 40 - (content.count() - 10);
//    if (checkboxh < 10 || checkboxw < 10) {
//        checkboxh = checkboxw = 10;
//    }
    checkboxh = checkboxw = 40;

    qDebug() << "wErrorTable::createErrorTableContent-CheckBox Size: " << checkboxh << " " << checkboxw;

//    int fontSize = 12 - (content.count() - 10);
//    QFont ubuntuFont("Ubuntu", (fontSize > 5)?fontSize:5, QFont::Bold);
    int fontSize = 20;
    QFont ubuntuFont("Ubuntu", fontSize, QFont::Bold);
    qDebug() << "wErrorTable::createErrorTableContent-Text Font Size: " << ubuntuFont.pointSize();

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
//        if (row >= (content.count() / 2)) {
//            row = 0;
//            col += 1;
//        }
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
    qDebug() << "wErrorTable::onCheckBoxChecked-CheckBox checked at: " << m_CurrentcheckBox;
    qDebug() << "wErrorTable::onCheckBoxChecked-Ma Loi: " << m_tableContent.at(index).first << " Ten Loi: " <<  m_tableContent.at(index).second;
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
        dataSession.settime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        dataSession.setMHCode(cKanbanParserUtils::getMH(m_MH));
        dataSession.setMHDatePrint(cKanbanParserUtils::getDatePrint(m_MH));
        dataSession.setMHNamePlate(cKanbanParserUtils::getMHNamePlate(m_MH));
        dataSession.setLine(cConfigureUtils::getLine());

        dataSession.setdeviceid(cConfigureUtils::getIpAddress());
        dataSession.setMaAB1(cAB1CodeParserUtils::getAB1(m_AB1));
        dataSession.setPrefixAB1(cAB1CodeParserUtils::getAB1Prefix(m_AB1));
        dataSession.setDatePrintAB1(cAB1CodeParserUtils::getAB1DatePrint(m_AB1));
        dataSession.setNamePlateAB1(cAB1CodeParserUtils::getAB1NamePlate(m_AB1));
        dataSession.setMaAB2(cAB2CodeParserUtils::getAB2(m_AB2));
        dataSession.setPrefixAB2(cAB2CodeParserUtils::getAB2Prefix(m_AB2));
        dataSession.setDatePrintAB2(cAB2CodeParserUtils::getAB2DatePrint(m_AB2));
        dataSession.setNamePlateAB2(cAB2CodeParserUtils::getAB2NamePlate(m_AB2));
        dataSession.setca(cStaffIDParserUtils::getCa(m_MNV));

        qDebug() << "wErrorTable::onButtonClicked-Ip Address: " << cConfigureUtils::getIpAddress();
        qDebug() << "wErrorTable::onButtonClicked-MH Date Print: " << cKanbanParserUtils::getDatePrint(m_MH);
        qDebug() << "wErrorTable::onButtonClicked-MH Name Plate: " << cKanbanParserUtils::getMHNamePlate(m_MH);
        qDebug() << "wErrorTable::onButtonClicked-AB1 Date Print: " << cAB1CodeParserUtils::getAB1DatePrint(m_AB1);
        qDebug() << "wErrorTable::onButtonClicked-AB1 Name Plate: " << cAB1CodeParserUtils::getAB1NamePlate(m_AB1);
        qDebug() << "wErrorTable::onButtonClicked-AB1 Prefix: " << cAB1CodeParserUtils::getAB1Prefix(m_AB1);
        qDebug() << "wErrorTable::onButtonClicked-AB1: " << cAB1CodeParserUtils::getAB1(m_AB1);
        qDebug() << "wErrorTable::onButtonClicked-AB2 Date Print: " << cAB2CodeParserUtils::getAB2DatePrint(m_AB2);
        qDebug() << "wErrorTable::onButtonClicked-AB2 Name Plate: " << cAB2CodeParserUtils::getAB2NamePlate(m_AB2);
        qDebug() << "wErrorTable::onButtonClicked-AB2: " << cAB2CodeParserUtils::getAB2(m_AB2);
        qDebug() << "wErrorTable::onButtonClicked-AB2 Prefix: " << cAB2CodeParserUtils::getAB2Prefix(m_AB2);
        QList<QPair<QString, int>> loi;
        for (int i = 0; i < m_TableRowWidgetList.count(); i++) {
            if (m_TableRowWidgetList.at(i)->isCheckBoxChecked()) {
                bool isConvertOK = false;
                QString maLoi = m_tableContent.at(i).first;
                int tempSLL = m_TableRowWidgetList.at(i)->getLineEditText().toInt(&isConvertOK, 10);
                if (isConvertOK) {
                    if (tempSLL == 0 ) {
                        inValidDataFound = true;
                        break;
                    } else {
                        SLL += tempSLL;
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
            m_MessageBox->setInformativeText("Số lượng lỗi phải khác 0, vui lòng kiểm tra lại");
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
