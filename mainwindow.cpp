#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "cStaffIDParserUtils.h"
#include "cKanbanParserUtils.h"
#include "cAB1CodeParserUtils.h"
#include "cAB2CodeParserUtils.h"
#include "cConfigureUtils.h"
#include "cMessageBox.h"
#include "cOperator.h"

#define SELECT_TABLE_PAGE           0
#define SCAN_STAFF_PAGE             1
#define SCAN_KANBAN_PAGE            2
#define CHECKING_KANBAN_PAGE        3
#define ERROR_TABLE_PAGE            4
#define CAMERA_STREAM_PAGE          5

#define STATE_IDLE                  0
#define STATE_CHECKING_MH           1
#define STATE_CHECKING_AB1          2
#define STATE_CHECKING_AB2          3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TitlebarText << "Vui lòng chọn bảng lỗi" << "Quét Mã Nhân Viên" << "Quét Mã Kanban" << "Kanban đang scan" << "Bảng lỗi" << "Chụp Ảnh";
    m_Database = cSQliteDatabase::instance();
    m_ContinueMH.clear();
    m_TenContinueousKanban.clear();


    m_SelectErrorWidget = new wSelectErrorTable();
    m_SelectErrorWidget->setErrorTableList(m_Database->getErrorTableName());
    m_SelectErrorWidget->createButtonTable();
    connect(m_SelectErrorWidget, SIGNAL(sigSelectLineClicked()), this, SLOT(onSelectLineClicked()));

    m_ScanStaffID = new wScanStaffID();

    m_SCanKanbanCode = new wScanKanbanCode();


    m_ScanStaffID->setStaffIDLable("SCAN");
    m_SCanKanbanCode->setKanbanLable("SCAN");
    m_SCanKanbanCode->setMaAB1("");
    m_SCanKanbanCode->setMaAB2("");

    m_CheckingKanban = new wCheckingKanban();
    connect(m_CheckingKanban, SIGNAL(sigOKCkicked()), this, SLOT(onCheckingKanbanOKClicked()));
    connect(m_CheckingKanban, SIGNAL(sigNGClicked()), this, SLOT(onCheckingKanbanNGClicked()));
    connect(m_CheckingKanban, SIGNAL(sigSettingClicked()), this, SLOT(onCheckingKanbanSettingClicked()));
    m_ErrorTable = new wErrorTable();

    m_CameraStream = new wCameraStream();
    connect(m_CameraStream, SIGNAL(sigCaptureImage()), this, SLOT(onCaptureImageRequest()));
    connect(m_CameraStream, SIGNAL(sigSaveImage()), this, SLOT(onSaveImageRequest()));
    connect(m_CameraStream, SIGNAL(sigStreamImage()), this, SLOT(onReCaptureImageRequest()));


    m_StackWidget = new QStackedWidget;
    m_StackWidget->addWidget(m_SelectErrorWidget);
    m_StackWidget->addWidget(m_ScanStaffID);
    m_StackWidget->addWidget(m_SCanKanbanCode);
    m_StackWidget->addWidget(m_CheckingKanban);
    m_StackWidget->addWidget(m_ErrorTable);
    m_StackWidget->addWidget(m_CameraStream);
    connect(m_SelectErrorWidget, SIGNAL(sigErrorTableSetected(QString)), this, SLOT(on_TableNameSelected(QString)));
    connect(m_ErrorTable, SIGNAL(finished()), this, SLOT(onErrorSessionFinished()));
    connect(m_ErrorTable, SIGNAL(sigCameraButtonClicked()), this, SLOT(onChupAnhClicked()));

    m_Titlebar = new cTitleBarWidget();
    m_Titlebar->hideCancelButton();
    connect(m_Titlebar, SIGNAL(sigCancelButtonClicked()), this, SLOT(on_CancelButtonClicked()));
    connect(m_Titlebar, SIGNAL(sigWifiConnected()), this, SLOT(onWifiConnected()));
    connect(m_Titlebar, SIGNAL(sigWifiDisconnected()), this, SLOT(onWifiDisconnected()));
    connect(m_Titlebar, SIGNAL(sigWifiWidgetClicked()), this, SLOT(onWifiWidgetClicked()));
    ui->vlStackWidget->setContentsMargins(10, 10, 10, 0);
    ui->vlStackWidget->addWidget(m_Titlebar);
    ui->vlStackWidget->addWidget(m_StackWidget);

    m_ScannerUtils = cScannerUtils::instance();
    connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

    m_UserSelectedTableName = "";
    setStackViewPage(SELECT_TABLE_PAGE);

    m_Camera = cCameraWorker::instance();
    connect(m_Camera, SIGNAL(sigSaveImageSuccess()), this, SLOT(onSaveImageSuccess()));

    workerThread = new QThread();
    serialportSender = cSerialPortSender::instance();
    serialportSender->moveToThread(workerThread);
    QObject::connect(workerThread, &QThread::started, serialportSender, &cSerialPortSender::main_loop);
    QObject::connect(serialportSender, &cSerialPortSender::finished, workerThread, &QThread::quit);
    workerThread->start();

    QObject::connect(serialportSender, SIGNAL(sigSerialPortConnected()), this, SLOT(onSerialPortConnected())) ;
    QObject::connect(serialportSender, SIGNAL(sigSerialPortDisconnected()), this, SLOT(onSerialPortDisconnected())) ;

    QObject::connect(serialportSender, SIGNAL(sigOperatorStatus(const QList<cOperator> &)), this, SLOT(onOperatorStatus(const QList<cOperator> &))) ;
}

MainWindow::~MainWindow()
{
    cScannerUtils::drop();
    cSQliteDatabase::drop();
    cCameraWorker::drop();
    delete ui;
}

void MainWindow::setStackViewPage(int page)
{
    if (m_StackWidget != nullptr && m_Titlebar != nullptr) {
        m_StackWidget->setCurrentIndex(page);
        m_Titlebar->SetCancelButtonIcon(QString(":/images/resources/human_pink.png"), QString(":/images/resources/human_gray.png"));
        if (m_StackWidget->currentIndex() > 1) {
            m_Titlebar->showCancelButton();
        } else {
            m_Titlebar->hideCancelButton();
        }
        if (page == ERROR_TABLE_PAGE || page == CAMERA_STREAM_PAGE) {
            m_Titlebar->setTitle(m_UserSelectedTableName);
            m_Titlebar->SetCancelButtonIcon(QString(":/images/resources/back_pink.png"), QString(":/images/resources/back_gray.png"));
        } else {
            m_Titlebar->setTitle(m_TitlebarText.at(page));
        }
        if (page == CHECKING_KANBAN_PAGE) {
            m_Titlebar->SetCancelButtonIcon(QString(":/images/resources/human_pink.png"), QString(":/images/resources/human_gray.png"));
            m_CurrentSessionData.setMaKanban(m_Kanbancode.toUpper());
            m_CurrentSessionData.setMNV(m_StaffID.toUpper());
            m_CurrentSessionData.setTenBangLoi(m_UserSelectedTableName.toUpper());
            m_CurrentSessionData.setMaAB1(m_MaAB1.toUpper());
            m_CurrentSessionData.setMaAB2(m_MaAB2.toUpper());
            m_CurrentSessionData.setMCUAction(m_MCUAction);
            m_Database->insertTempSession(m_CurrentSessionData);
            if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
                m_CheckingKanban->onBtnOKClicked();
            }
        }
    }

}

void MainWindow::setOperator(quint8 Operator)
{

}

void MainWindow::onWifiConnected()
{
    //use when upload data
    m_WifiConnected = true;
    m_Titlebar->setSyncingDoneState();

}

void MainWindow::onWifiDisconnected()
{
    // use when upload data
    m_WifiConnected = false;
    m_Titlebar->setSyncingDoneStateWithWarn();
}

void MainWindow::onWifiWidgetClicked()
{
    cMessageBox *m_MessageBox = new cMessageBox();
    m_MessageBox->setText("Cài đặt Wi-Fi");
    m_MessageBox->setInformativeText("Bạn có chắc muốn thoát ứng dụng để cài đặt Wi-Fi");
    m_MessageBox->setIcon(QPixmap(":/images/resources/warningicon_80x80.png"));
    int retVal = m_MessageBox->exec();
    delete m_MessageBox;
    if (retVal == QDialog::Accepted) {
        QApplication::quit();
    }
}

void MainWindow::onCheckingKanbanOKClicked()
{
    //Write kanban data to database, kanban no error

    cDataSession dataSession;
    cDataSessionActivating tempDataSession = m_Database->getTempSession();
    dataSession.setHinh(tempDataSession.getPicturesList());
    dataSession.setmnv(cStaffIDParserUtils::getMNV(m_StaffID));
    dataSession.settime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    dataSession.setMHCode(cKanbanParserUtils::getMH(m_Kanbancode));
    dataSession.setMHDatePrint(cKanbanParserUtils::getDatePrint(m_Kanbancode));
    dataSession.setMHNamePlate(cKanbanParserUtils::getMHNamePlate(m_Kanbancode));
    dataSession.setLine(cConfigureUtils::getLine());

    dataSession.setca(cStaffIDParserUtils::getCa(m_StaffID));
    dataSession.setdeviceid(cConfigureUtils::getIpAddress());
    dataSession.setMaAB1(cAB1CodeParserUtils::getAB1(m_MaAB1));
    dataSession.setPrefixAB1(cAB1CodeParserUtils::getAB1Prefix(m_MaAB1));
    dataSession.setDatePrintAB1(cAB1CodeParserUtils::getAB1DatePrint(m_MaAB1));
    dataSession.setNamePlateAB1(cAB1CodeParserUtils::getAB1NamePlate(m_MaAB1));
    dataSession.setMaAB2(cAB2CodeParserUtils::getAB2(m_MaAB2));
    dataSession.setPrefixAB2(cAB2CodeParserUtils::getAB2Prefix(m_MaAB2));
    dataSession.setDatePrintAB2(cAB2CodeParserUtils::getAB2DatePrint(m_MaAB2));
    dataSession.setNamePlateAB2(cAB2CodeParserUtils::getAB2NamePlate(m_MaAB2));

    qDebug() << "Ip Address: " << cConfigureUtils::getIpAddress();
    qDebug() << "MH Date Print: " << cKanbanParserUtils::getDatePrint(m_Kanbancode);
    qDebug() << "MH Name Plate: " << cKanbanParserUtils::getMHNamePlate(m_Kanbancode);
    qDebug() << "AB1 Date Print: " << cAB1CodeParserUtils::getAB1DatePrint(m_MaAB1);
    qDebug() << "AB1 Name Plate: " << cAB1CodeParserUtils::getAB1NamePlate(m_MaAB1);
    qDebug() << "AB1 Prefix: " << cAB1CodeParserUtils::getAB1Prefix(m_MaAB1);
    qDebug() << "AB1: " << cAB1CodeParserUtils::getAB1(m_MaAB1);
    qDebug() << "AB2 Date Print: " << cAB2CodeParserUtils::getAB2DatePrint(m_MaAB2);
    qDebug() << "AB2 Name Plate: " << cAB2CodeParserUtils::getAB2NamePlate(m_MaAB2);
    qDebug() << "AB2: " << cAB2CodeParserUtils::getAB2(m_MaAB2);
    qDebug() << "AB2 Prefix: " << cAB2CodeParserUtils::getAB2Prefix(m_MaAB2);
    qDebug() << "HINH count: " << tempDataSession.getPicturesList().count();
    for(int i = 0; i < tempDataSession.getPicturesList().count(); i++)
    {
        qDebug() << "HINH: " << tempDataSession.getPicturesList().at(i);
    }

    QList<QPair<QString, int>> loi;
    dataSession.setloi(loi);
    bool retVal = m_Database->insertHistoryTransaction(dataSession, cSQliteDatabase::UNSUBMITED);
    if (retVal)
        m_Database->deleteTempSession();
    onErrorSessionFinished();
}

void MainWindow::onCheckingKanbanNGClicked()
{
    nextStackView();
}

void MainWindow::onCheckingKanbanSettingClicked()
{
    //Show Setting screen
    QString password = cConfigureUtils::getPassword();

    //Hien thi hop thoai nhap pass
    m_passwordDialog = new cPasswordDialog();
#ifdef BUILD_PC
    m_passwordDialog->setFixedSize(800,480);
#else
    m_passwordDialog->setWindowState( m_passwordDialog->windowState() | Qt::WindowFullScreen);
#endif
    int result = m_passwordDialog->exec();
    if (result == QDialog::Accepted) {
        QString userPass = m_passwordDialog->getPassword();
        if (QString::compare(userPass, password) == 0) {
            m_SettingDialog = new cSettingDialog();
#ifdef BUILD_PC
            m_SettingDialog->setFixedSize(800,480);
#else
            m_SettingDialog->setWindowState( m_SettingDialog->windowState() | Qt::WindowFullScreen);
#endif
            m_SettingDialog->exec();
            delete m_SettingDialog;
            if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
                m_CheckingKanban->onBtnOKClicked();
            }
        } else {
            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Sai Mật Khẩu");
            m_MessageBox->setInformativeText("Sai Mật Khẩu, vui lòng kiểm tra lại");
            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
            m_MessageBox->setHideRejectButton();
            m_MessageBox->exec();
            delete m_MessageBox;
        }
    }
    delete m_passwordDialog;
}

void MainWindow::onScannerReady(const QString &data)
{
    qDebug() << "Scanner From Main Windows : " << data;
    qDebug() << "Scanner From Main Windows : " << data.length();
    static int mySomaAB = 0;
    static QString MCUAction = "";
    if (!m_UserSelectedTableName.isEmpty() && m_StackWidget->currentIndex() == SCAN_KANBAN_PAGE && data.length() == 23 && data.at(0).toUpper() != "A" && data.at(1).toUpper() != "B" ) {

        if(m_MyState == STATE_CHECKING_MH)
        {
            qDebug() << "Scanner From Main Windows : " << m_ContinueMH.length();
            qDebug() << "Scanner From Main Windows : " << cConfigureUtils::getNumOfSameScan();
            if(((m_ContinueMH.contains(data.toUpper()) == false) && (m_ContinueMH.length() == 0)) || ((m_ContinueMH.contains(data.toUpper()) == true) && (m_ContinueMH.length() < cConfigureUtils::getNumOfSameScan())))
            {
                if(!m_TenContinueousKanban.contains(data.toUpper()))
                {
                    qDebug() << "Found MH Code: " << data;
                    m_Kanbancode = data.toUpper();
                    m_SCanKanbanCode->setKanbanLable(m_Kanbancode.toUpper());
                    m_CheckingKanban->setLine(cConfigureUtils::getLine());
                    m_ErrorTable->setMH(m_Kanbancode.toUpper());
                    m_CheckingKanban->setMKB(m_Kanbancode.toUpper());

                    QList<cDataMH> myDataMH = m_Database->getNewDataMH();

                    for(int i = 0; i < myDataMH.length(); i++)
                    {
                        if(myDataMH[i].getMaHang() == m_Kanbancode.toUpper())
                        {
                            qDebug() << "Detect MH Code in DB ";
                            mySomaAB = myDataMH[i].getSoMaAB();
                            MCUAction = myDataMH[i].getThaoTacMCU();
                            qDebug() << "Set MCU Data: " << MCUAction.toLatin1();
                            serialportSender->setBoardData(MCUAction.toLatin1());
                            QMap<QString, QStringList> finalMCUAct;
                            QStringList myMCU = MCUAction.split("OP");
                            myMCU.removeFirst();
                            m_MCUAction = "";
                            m_MCUAction = MCUAction;
                            int numOfBox = myMCU.count();
                            m_MaAB1 = "";
                            m_MaAB2 = "";
                            m_MaAB1 = myDataMH[i].getMaAB1().toUpper();
                            m_MaAB2 = myDataMH[i].getMaAB2().toUpper();
                            m_CheckingKanban->setMAB1(m_MaAB1);
                            m_CheckingKanban->setMAB2(m_MaAB2);
                            m_CheckingKanban->createMCUActionBox(numOfBox);
                            m_ErrorTable->setAB1(m_MaAB1);
                            m_ErrorTable->setAB2(m_MaAB2);
                            qDebug() << "So ma AB: " << mySomaAB;
                            qDebug() << "Ma AB 1: " << m_MaAB1;
                            qDebug() << "Ma AB 2:" << m_MaAB2;

                        }
                    }
                    if(mySomaAB > 0)
                    {
                        m_SCanKanbanCode->setMaAB1("SCAN");
                        m_MyState = STATE_CHECKING_AB1;
                    }
                    else
                    {
                        mySomaAB = 0;
                        if(m_ContinueMH.length() == (cConfigureUtils::getNumOfSameScan() - 1))
                        {
                            m_ContinueMH.clear();
                            m_TenContinueousKanban.append(m_Kanbancode);
                            if (m_TenContinueousKanban.count() >= 10) {
                                m_TenContinueousKanban.removeFirst();
                            }
                        }
                        else
                        {
                            m_ContinueMH.append(m_Kanbancode);
                        }
                        QTimer::singleShot(200, this, SLOT(nextStackView()));
                    }
                }
                else
                {
                    disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

                    cMessageBox *m_MessageBox = new cMessageBox();
                    m_MessageBox->setText("Lỗi Scan Kanban");
                    m_MessageBox->setInformativeText("10 lần scan kanban liên tiếp không được trùng nhau, vui lòng kiểm tra lại");
                    m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
                    m_MessageBox->setHideRejectButton();
                    m_MessageBox->exec();
                    delete m_MessageBox;
                    connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
                }

            }
            else
            {
                disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

                cMessageBox *m_MessageBox = new cMessageBox();
                m_MessageBox->setText("Lỗi Scan Mã Hàng");
                m_MessageBox->setInformativeText("Mã hàng không hợp lệ, vui lòng kiểm tra lại");
                m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
                m_MessageBox->setHideRejectButton();
                m_MessageBox->exec();
                delete m_MessageBox;
                connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            }



        }
        else
        {
            disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Lỗi Scan Kanban");
            m_MessageBox->setInformativeText("Cần scan mã AB, vui lòng kiểm tra lại");
            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
            m_MessageBox->setHideRejectButton();
            m_MessageBox->exec();
            delete m_MessageBox;
            connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
        }
    }
    else if(!m_UserSelectedTableName.isEmpty() && m_StackWidget->currentIndex() == SCAN_KANBAN_PAGE && data.at(0).toUpper() == "A" && data.at(1).toUpper() == "B")
    {
        if(m_MyState == STATE_CHECKING_AB1)
        {

            if(m_MaAB1 == data.toUpper())
            {
                if(mySomaAB == 1)
                {
                    m_SCanKanbanCode->setMaAB1(data.toUpper());
                    mySomaAB = 0;
                    qDebug() << "Send data to board ";
                    serialportSender->requestMethod(cSerialPortSender::SENDDATA);
                    m_MyState = STATE_CHECKING_MH;
                    QTimer::singleShot(200, this, SLOT(nextStackView()));

                    if(m_ContinueMH.length() == (cConfigureUtils::getNumOfSameScan() - 1))
                    {
                        m_ContinueMH.clear();
                        m_TenContinueousKanban.append(m_Kanbancode);
                        if (m_TenContinueousKanban.count() >= 10) {
                            m_TenContinueousKanban.removeFirst();
                        }
                    }
                    else
                    {
                        m_ContinueMH.append(m_Kanbancode);
                    }
                }
                else if(mySomaAB == 2)
                {
                    m_SCanKanbanCode->setMaAB1(data.toUpper());
                    m_MyState = STATE_CHECKING_AB2;
                    m_SCanKanbanCode->setMaAB2("SCAN");
                }
                else
                {

                }

            }
            else
            {
                disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

                cMessageBox *m_MessageBox = new cMessageBox();
                m_MessageBox->setText("Lỗi Scan Kanban");
                m_MessageBox->setInformativeText("Mã AB 1 không đúng, vui lòng kiểm tra lại");
                m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
                m_MessageBox->setHideRejectButton();
                m_MessageBox->exec();
                delete m_MessageBox;
                connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            }



        }
        else if(m_MyState == STATE_CHECKING_AB2)
        {
            if(m_MaAB2 == data.toUpper())
            {
                m_SCanKanbanCode->setMaAB2(data.toUpper());
                mySomaAB = 0;
                qDebug() << "Send data to board ";
                serialportSender->requestMethod(cSerialPortSender::SENDDATA);
                if(m_ContinueMH.length() == (cConfigureUtils::getNumOfSameScan() - 1))
                {
                    m_ContinueMH.clear();
                    m_TenContinueousKanban.append(m_Kanbancode);
                    if (m_TenContinueousKanban.count() >= 10) {
                        m_TenContinueousKanban.removeFirst();
                    }
                }
                else
                {
                    m_ContinueMH.append(m_Kanbancode);
                }
                m_MyState = STATE_CHECKING_MH;
                QTimer::singleShot(200, this, SLOT(nextStackView()));
            }
            else
            {
                disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

                cMessageBox *m_MessageBox = new cMessageBox();
                m_MessageBox->setText("Lỗi Scan Kanban");
                m_MessageBox->setInformativeText("Mã AB 2 không đúng, vui lòng kiểm tra lại");
                m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
                m_MessageBox->setHideRejectButton();
                m_MessageBox->exec();
                delete m_MessageBox;
                connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            }

        }
        else
        {
            disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Lỗi Scan Kanban");
            m_MessageBox->setInformativeText("Cần scan mã hàng, vui lòng kiểm tra lại");
            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
            m_MessageBox->setHideRejectButton();
            m_MessageBox->exec();
            delete m_MessageBox;
            connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
        }
    }
    else if (!m_UserSelectedTableName.isEmpty() && (m_StackWidget->currentIndex() == SCAN_STAFF_PAGE) && (data.length() == 10) && data.contains("0000")) {
        qDebug() << "Found Staff ID: " << data;
        m_StaffID = data;
        m_ScanStaffID->setStaffIDLable(m_StaffID.toUpper());
        m_ErrorTable->setMNV(m_StaffID.toUpper());
        m_CheckingKanban->setMNV(m_StaffID.toUpper());
        m_CurrentSessionData = m_Database->getTempSession();
        if (m_CurrentSessionData.getMaKanban().isEmpty()) {
            QTimer::singleShot(200, this, SLOT(nextStackView()));

        }
        else {
            m_Kanbancode = m_CurrentSessionData.getMaKanban();
            m_MaAB1 = m_CurrentSessionData.getMaAB1();
            m_MaAB2 = m_CurrentSessionData.getMaAB2();
            QString myMCUAction = m_CurrentSessionData.getMCUAction();
            m_ErrorTable->setMH(m_Kanbancode.toUpper());
            m_CheckingKanban->setMKB(m_Kanbancode.toUpper());
            m_CheckingKanban->setMAB1(m_MaAB1.toUpper());
            m_CheckingKanban->setMAB2(m_MaAB2.toUpper());
            m_CheckingKanban->setLine(cConfigureUtils::getLine());
            m_ErrorTable->setAB1(m_MaAB1.toUpper());
            m_ErrorTable->setAB2(m_MaAB2.toUpper());
            disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Kanban đang kiểm tra dang dở");
            m_MessageBox->setInformativeText(QString("Kanban có mã %1 đang kiểm tra dang dở. Bạn có muốn tiếp tục kiểm tra?").arg(m_Kanbancode.toUpper()));
            m_MessageBox->setIcon(QPixmap(":/images/resources/infoicon_80x80.png"));
            int retVal = m_MessageBox->exec();
            delete m_MessageBox;
            connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            if (retVal == QDialog::Accepted) {
                m_ContinueMH.append(m_Kanbancode);

                qDebug() << "Set MCU Data: " << myMCUAction.toLatin1();
                serialportSender->setBoardData(myMCUAction.toLatin1());
                QMap<QString, QStringList> finalMCUAct;
                QStringList myMCU = myMCUAction.split("OP");
                myMCU.removeFirst();
                qDebug() << myMCU.count();

                int numOfBox = myMCU.count();
                m_CheckingKanban->createMCUActionBox(numOfBox);
                serialportSender->requestMethod(cSerialPortSender::SENDDATA);
                setStackViewPage(m_StackWidget->currentIndex() + 2);

            } else {
                nextStackView();
            }
        }
    }
}

void MainWindow::on_TableNameSelected(QString tableName)
{
        qDebug() << "Selected Table Name: " << tableName;
        m_UserSelectedTableName = tableName;
        m_ErrorTable->clearErrorTableContent();
        m_ErrorTable->createErrorTableContent(m_Database->getErrorTablecontent(tableName));
        nextStackView();
}

void MainWindow::on_CancelButtonClicked()
{
    if (m_StackWidget->currentIndex() == CAMERA_STREAM_PAGE) {
        m_Camera->requestMethod(cCameraWorker::SLEEP);
        previousStackView();
    } else if (m_StackWidget->currentIndex() == ERROR_TABLE_PAGE) {
        previousStackView();
    }
    else {
        m_CheckingKanban->cancelOperation();
        m_StaffID.clear();
        m_Kanbancode.clear();
        m_ScanStaffID->setStaffIDLable("SCAN");
        m_SCanKanbanCode->setKanbanLable("SCAN");
        m_SCanKanbanCode->setMaAB1("");
        m_SCanKanbanCode->setMaAB2("");
        m_ErrorTable->clearCheckedItems();
        setStackViewPage(SCAN_STAFF_PAGE);
    }

}

void MainWindow::nextStackView()
{
    if (m_StackWidget->currentIndex() < (m_StackWidget->count() - 1))
        setStackViewPage(m_StackWidget->currentIndex() + 1);
}

void MainWindow::previousStackView()
{
    if (m_StackWidget->currentIndex() > 0)
        setStackViewPage(m_StackWidget->currentIndex() - 1);
}

void MainWindow::onErrorSessionFinished()
{
    m_SCanKanbanCode->setKanbanLable("SCAN");
    m_SCanKanbanCode->setMaAB1("");
    m_SCanKanbanCode->setMaAB2("");
    m_Kanbancode = "";
    setStackViewPage(SCAN_KANBAN_PAGE);
}

void MainWindow::onDatabseSyncing()
{
    m_Titlebar->setSyncingState();
}

void MainWindow::onDatabaseSyncDone()
{
    m_Titlebar->setSyncingDoneState();
}

void MainWindow::onChupAnhClicked()
{
    if (m_Camera != nullptr) {
         qDebug() << "Chup Anh Clicked";
         m_Camera->setKanbanCode(cKanbanParserUtils::getMH(m_Kanbancode));
         m_Camera->requestMethod(cCameraWorker::STREAMANH);
         nextStackView();
    }
}

void MainWindow::onCaptureImageRequest()
{
    if (m_Camera != nullptr) {
        m_Camera->requestMethod(cCameraWorker::CHUPANH);

    }
}

void MainWindow::onSaveImageRequest()
{
    if (m_Camera != nullptr) {
         qDebug() << "Save Image Clicked";
         m_Camera->requestMethod(cCameraWorker::LUUANH);
    }
}

void MainWindow::onReCaptureImageRequest()
{
    if (m_Camera != nullptr) {
         qDebug() << "Chup Anh Lai Clicked";
         m_Camera->requestMethod(cCameraWorker::STREAMANH);
    }
}

void MainWindow::onSaveImageSuccess()
{
    QTimer::singleShot(1000, this, SLOT(previousStackView()));
}

void MainWindow::onFramedataReady(QPixmap pixmap)
{
    if (m_CameraStream != nullptr) {
        m_CameraStream->setImage(pixmap);
    }
}

void MainWindow::onSelectLineClicked()
{
    m_SelectLineDialog = new wSelectLine();
//    m_SelectLineDialog->setFixedSize(800,600);


    m_SelectLineDialog->exec();
    delete m_SelectLineDialog;
}

void MainWindow::onSerialPortConnected()
{
    qDebug() << "Main: Capture signal Serial Port Connected";
    m_isMCUConnected = true;
    if(m_StackWidget->currentIndex() == CHECKING_KANBAN_PAGE)
    {
        serialportSender->requestMethod(cSerialPortSender::SENDDATA);
    }
}

void MainWindow::onSerialPortDisconnected()
{
    qDebug() << "Main: Capture signal Serial Port Disonnected";
    m_isMCUConnected = false;
}

void MainWindow::onOperatorStatus(const QList<cOperator> &status)
{
    foreach (cOperator op, status) {
//        qDebug() << "Oprator: " << op.getOperator(); //0 - 7
//        qDebug() << "status: " << op.getStatusCode(); // 1 succcessful 0 failed
//        qDebug() << "Num OP: " << op.getNumOP(); // operators
        if(op.getStatusCode() == 1)
        {
            if(m_StackWidget->currentIndex() == CHECKING_KANBAN_PAGE)
            {
                m_CheckingKanban->setOperatorStatus(op.getOperator(), m_isMCUConnected);
            }
        }
    }
}
