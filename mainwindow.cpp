#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "cStaffIDParserUtils.h"
#include "cKanbanParserUtils.h"
#include "cConfigureUtils.h"
#include "cMessageBox.h"

#define SELECT_TABLE_PAGE           0
#define SCAN_STAFF_PAGE             1
#define SCAN_KANBAN_PAGE            2
#define CHECKING_KANBAN_PAGE        3
#define ERROR_TABLE_PAGE            4
#define CAMERA_STREAM_PAGE          5

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TitlebarText << "Vui lòng chọn bảng lỗi" << "Quét Mã Nhân Viên" << "Quét Mã Kanban" << "Kanban đang scan" << "Bảng lỗi" << "Chụp Ảnh";
    m_TenContinueousKanban.clear();
    m_Database = cSQliteDatabase::instance();


    m_SelectErrorWidget = new wSelectErrorTable();
    m_SelectErrorWidget->setErrorTableList(m_Database->getErrorTableName());
    m_SelectErrorWidget->createButtonTable();

    m_ScanStaffID = new wScanStaffID();

    m_SCanKanbanCode = new wScanKanbanCode();

    m_ScanStaffID->setStaffIDLable("SCAN");
    m_SCanKanbanCode->setKanbanLable("SCAN");

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
            m_Database->insertTempSession(m_CurrentSessionData);
            if (QString::compare(cConfigureUtils::getManualMode(), "1") != 0) {
                m_CheckingKanban->onBtnOKClicked();
            }
        }
    }

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
    dataSession.settime(QDateTime::currentDateTime().toString("ddMMyyhhmmss"));
    dataSession.setseibango(cKanbanParserUtils::getSeibango(m_Kanbancode));
    dataSession.setsoto(cKanbanParserUtils::getSoTo(m_Kanbancode).toInt());
    dataSession.setsosoi(cKanbanParserUtils::getSoSoi(m_Kanbancode).toInt());
    dataSession.setblock(cKanbanParserUtils::getTenBlock(m_Kanbancode.toUpper()));
    dataSession.setca(cStaffIDParserUtils::getCa(m_StaffID));
    dataSession.setdeviceid(cConfigureUtils::getWifiMac());
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
    if (!m_UserSelectedTableName.isEmpty() && m_StackWidget->currentIndex() == SCAN_KANBAN_PAGE && data.length() == 21 && data.at(0) != "A" && data.at(1) != "B") {
        qDebug() << "Found MH Code: " << data;
//        if (!m_TenContinueousKanban.contains(data.toUpper())) {
//            m_Kanbancode = data.toUpper();
//            m_TenContinueousKanban.append(m_Kanbancode);
//            if (m_TenContinueousKanban.count() >= 10) {
//                m_TenContinueousKanban.removeFirst();
//            }
//            m_SCanKanbanCode->setKanbanLable(m_Kanbancode.toUpper());
//            m_ErrorTable->setMH(m_Kanbancode.toUpper());
//            m_CheckingKanban->setMKB(m_Kanbancode.toUpper());
//            QTimer::singleShot(200, this, SLOT(nextStackView()));
//        } else {
//            disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));

//            cMessageBox *m_MessageBox = new cMessageBox();
//            m_MessageBox->setText("Lỗi Scan Kanban");
//            m_MessageBox->setInformativeText("10 lần scan kanban liên tiếp không được trùng nhau, vui lòng kiểm tra lại");
//            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
//            m_MessageBox->setHideRejectButton();
//            m_MessageBox->exec();
//            delete m_MessageBox;
//            connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
//        }
    } else if (!m_UserSelectedTableName.isEmpty() && (m_StackWidget->currentIndex() == SCAN_STAFF_PAGE) && (data.length() == 10) && data.contains("0000")) {
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
            m_ErrorTable->setMH(m_Kanbancode.toUpper());
            m_CheckingKanban->setMKB(m_Kanbancode.toUpper());
            disconnect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Kanban đang kiểm tra dang dở");
            m_MessageBox->setInformativeText(QString("Kanban có mã %1 đang kiểm tra dang dở. Bạn có muốn tiếp tục kiểm tra?").arg(m_Kanbancode.toUpper()));
            m_MessageBox->setIcon(QPixmap(":/images/resources/infoicon_80x80.png"));
            int retVal = m_MessageBox->exec();
            delete m_MessageBox;
            connect(m_ScannerUtils, SIGNAL(onDataDetected(QString)), this, SLOT(onScannerReady(QString)));
            if (retVal == QDialog::Accepted) {
                m_TenContinueousKanban.append(m_Kanbancode);
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
         m_Camera->setKanbanCode(m_Kanbancode);
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
