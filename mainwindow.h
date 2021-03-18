#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QThread>
#include <cScannerUtils.h>
#include <wSelectErrorTable.h>
#include <cDataSession.h>
#include <cSqliteDatabase.h>
#include <wScanStaffID.h>
#include <wScanKanbanCode.h>
#include <wErrorTable.h>
#include <cTitleBarWidget.h>
#include <wCheckingKanban.h>
#include "cSettingDialog.h"
#include "cDataSessionActivating.h"
#include "cCameraWorker.h"
#include "wCameraStream.h"
#include "cPasswordDialog.h"
#include "wSelectLine.h"
#include "cSerialPortSender.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void setStackViewPage(int page);
    void setOperator(quint8 Operator);
    void sleep_ms(int time_ms);
private:
    Ui::MainWindow *ui;
    cScannerUtils *m_ScannerUtils;
    cSQliteDatabase *m_Database;
    cSettingDialog *m_SettingDialog = nullptr;
    wSelectLine *m_SelectLineDialog = nullptr;
    QStackedWidget *m_StackWidget;
    wSelectErrorTable *m_SelectErrorWidget;
    wScanStaffID *m_ScanStaffID;
    wScanKanbanCode *m_SCanKanbanCode;
    wErrorTable *m_ErrorTable;
    wCheckingKanban *m_CheckingKanban = nullptr;
    wCameraStream *m_CameraStream = nullptr;
    cTitleBarWidget *m_Titlebar = nullptr;
    cCameraWorker *m_Camera = nullptr;
    cPasswordDialog *m_passwordDialog = nullptr;
    cSerialPortSender *serialportSender = nullptr;
    QThread *workerThread = nullptr;
    int m_NumberOfReturnScanMH = 1;
    int m_NumberOfSameMH = 1;
    int m_MyState = 1;
    int m_PreState = 0;

    QString m_UserSelectedTableName;
    QString m_StaffID;
    QString m_Kanbancode;
    QString m_MaAB1;
    QString m_MaAB2;
    QString m_MCUAction;
    QString m_Magic;
    QString m_Dongmoc;
    QStringList m_TitlebarText;
    QStringList m_ContinueMH;
    QStringList m_TenContinueousKanban;
    bool m_WifiConnected = false;
    bool m_isMCUConnected = false;
    cDataSessionActivating m_CurrentSessionData;
private slots:
    void onWifiConnected();
    void onWifiDisconnected();
    void onWifiWidgetClicked();

    void onCheckingKanbanOKClicked();
    void onCheckingKanbanNGClicked();
    void onCheckingKanbanSettingClicked();

    void onCheckingKanbanFinishOperator();
    void onCheckingKanbanFinishMagic();
    void onDongmocFinish();

    void onScannerReady(const QString &data);
    void on_TableNameSelected(QString tableName);
    void on_CancelButtonClicked();
    void nextStackView();
    void previousStackView();
    void onErrorSessionFinished();

    void onDatabseSyncing();
    void onDatabaseSyncDone();
    void onChupAnhClicked();
    void onCaptureImageRequest();
    void onSaveImageRequest();
    void onReCaptureImageRequest();
    void onSaveImageSuccess();
    void onFramedataReady(QPixmap pixmap);
    void onSelectLineClicked();
    void onSerialPortConnected();
    void onSerialPortDisconnected();
    void onOperatorStatus(const QList<cOperator> &status);
    void onMagicStatus(bool status);
    void onCarpentryStatus(bool status);
};

#endif // MAINWINDOW_H
