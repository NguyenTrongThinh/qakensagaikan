#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QThread>
#include <QList>
#include <cServerUtils.h>
#include <cDataSession.h>
#include <cSqliteDatabase.h>
#include <cConfigureUtils.h>
#include <cScannerUtils.h>
#include <cAutoSync.h>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDateTime>
#include <QJsonArray>
#include "cCameraWorker.h"
#include "cPicturesUtils.h"
#include "cPicturesData.h"
#include <QDebug>
#include <QTimer>
#include "cdatamh.h"


void myMessageHandler(QtMsgType type,const QMessageLogContext &context,const QString &msg)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile outFile(homeFolder + "/" + "kensagaikanlog.txt");

    if (outFile.exists()) {
        qint64 sizeInByte = outFile.size();
        if (sizeInByte > 104857600) {
            outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        } else {
            outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        }
    } else {
         outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }


    QTextStream ts(&outFile);
    QString txt;
    switch (type) {
    case QtInfoMsg:
        txt = QString("Info: %1 %2 %3:").arg(context.file).arg(context.line).arg(context.function);
        ts << txt <<"\t"<< QString(msg)<<endl;
        break;
    case QtDebugMsg:
        txt = QString("Debug: %1 %2 %3:").arg(context.file).arg(context.line).arg(context.function);
        ts << txt <<"\t"<< QString(msg)<<endl;
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1 %2 %3:").arg(context.file).arg(context.line).arg(context.function);
        ts << txt <<"\t"<< QString(msg)<<endl;
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1 %2 %3:").arg(context.file).arg(context.line).arg(context.function);
        ts << txt <<"\t"<< QString(msg)<<endl;
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1 %2 %3:").arg(context.file).arg(context.line).arg(context.function);
        ts << txt <<"\t"<< QString(msg)<<endl;
        abort();
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifndef BUILD_PC
    qInstallMessageHandler(myMessageHandler);
#endif
   // QPixmap pixmap(":/splash/resources/splash.png");
//    QSplashScreen splash(pixmap);

//    QFont splashFont;
//    splashFont.setFamily("Arial");
//    splashFont.setBold(true);
//    splashFont.setPixelSize(20);
//    splashFont.setStretch(125);

//    splash.setFont(splashFont);
//    splash.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen);
//    splash.show();

    qRegisterMetaType<cDataSession>("cDataSession");
    qRegisterMetaType<QList<cDataSession>>("cDataSession");
    qRegisterMetaType<cDataMH>("cDataMH");
    qRegisterMetaType<QList<cDataMH>>("cDataMH");
    qRegisterMetaType<cPicturesData>("cPicturesData");
    qRegisterMetaType<QList<cPicturesData>>("cPicturesData");
//    splash.showMessage("Khởi tạo kết nối máy scan barcode ...", Qt::AlignHCenter | Qt::AlignTop, Qt::blue);
    a.processEvents();

    QThread *m_ScannerWorkerThread = new QThread();
    cScannerUtils *m_ScannerWorker = cScannerUtils::instance();
    m_ScannerWorker->moveToThread(m_ScannerWorkerThread);
    QObject::connect(m_ScannerWorkerThread, SIGNAL(started()), m_ScannerWorker, SLOT(mainLoop()));
    QObject::connect(m_ScannerWorker, SIGNAL(finished()), m_ScannerWorkerThread, SLOT(quit()), Qt::DirectConnection);
    m_ScannerWorkerThread->start();

//    splash.showMessage("Đang kết nối cơ sở dữ liệu...", Qt::AlignHCenter | Qt::AlignTop, Qt::blue);
    a.processEvents();

    //Test Code goes here
    //cConfigureUtils::setPicturesDir("/home/thinhnguyen/Pictures/Webcam");
    //End of test code here

    cSQliteDatabase *m_SQLiteDatabase = cSQliteDatabase::instance();
    if (m_SQLiteDatabase->initDatabase().type() == QSqlError::NoError)
    {
        m_SQLiteDatabase->clearNewError();
        m_SQLiteDatabase->clearDataMH();
        m_SQLiteDatabase->createTransactionTable();
        m_SQLiteDatabase->createTempSessionTable();
        m_SQLiteDatabase->createPicturesLocationTable();
//        splash.showMessage("Đang xóa cơ sở dữ liệu cũ...", Qt::AlignHCenter | Qt::AlignTop, Qt::blue);
        a.processEvents();
        m_SQLiteDatabase->deleteOldSubmitedTransaction();
        QList<cPicturesData> oldPictures = m_SQLiteDatabase->getOldSubmitedPicturesTransaction();
        foreach (cPicturesData picData, oldPictures) {
            cPicturesUtils::deletePicture(picData.getLocationOnDisk());
        }
        m_SQLiteDatabase->deleteOldSubmitedPicturesTransaction();
        // Delete old picture database and pictures here;

        cServerUtils *m_serverutils = cServerUtils::instance();

//        splash.showMessage("Cập nhật bảng mã lỗi...", Qt::AlignHCenter | Qt::AlignTop, Qt::blue);
        a.processEvents();
        QString serveraddr =  cConfigureUtils::getServerAdddress();
        qDebug() << "Server Address: " << serveraddr;
        m_serverutils->setServerAddress(serveraddr);
        //Start Timer here
        QJsonArray mahangTable = m_serverutils->getMHTable();
        QList<cDataMH> tableMH = m_serverutils->getListDataMH(mahangTable);
        if(tableMH.count() > 0)
        {
            bool delVal = m_SQLiteDatabase->deleteOldMHTable();
            qDebug() << "DEL MH Table : " << delVal;
            bool retVal = m_SQLiteDatabase->createTempMHTable();
            qDebug() << "Cretate MH Table : " << retVal;
            retVal = m_SQLiteDatabase->insertAllMHIntoTable(tableMH);
            qDebug() << "Insert Data to MH Table : " << retVal;

        }

        QJsonArray errorTable = m_serverutils->getErrorTable();
        QStringList tableName = m_serverutils->getListTableName(errorTable);
        if (tableName.count() > 0) {
            bool delVal = m_SQLiteDatabase->deleteErrorTable();
            qDebug() << "DEL Error Table : " << delVal;
        }
        foreach(QString name, tableName) {

            bool retVal = m_SQLiteDatabase->createErrorTable(name);
            qDebug() << "Cretate Error Table : " << retVal;
            retVal = m_SQLiteDatabase->insertAllIntoTable(name, m_serverutils->getListTablecontent(errorTable, name));
            qDebug() << "Insert Data to ERROR Table : " << retVal;

        }
        qDebug() << "Found " << m_SQLiteDatabase->getNewError().count() << "new error";
        if (tableName.count() <= 0) {
           m_SQLiteDatabase->getOldMHTable();
           qDebug() << "Unable to connect to server. contimue in ofline mode";
        }
        if (m_SQLiteDatabase->getNewError().count() > 0)
        {
            qDebug() << "Found New Error: " << m_SQLiteDatabase->getNewError().count();
        }


//        splash.showMessage("Khởi tạo Camera", Qt::AlignHCenter | Qt::AlignTop, Qt::blue);
        a.processEvents();
        //Start Camera Thread
        QThread m_CameraThread;
        cCameraWorker *m_CameraWorker = cCameraWorker::instance();
        m_CameraWorker->moveToThread(&m_CameraThread);
        QObject::connect(&m_CameraThread, SIGNAL(started()), m_CameraWorker, SLOT(mainLoop()));
        QObject::connect(m_CameraWorker, SIGNAL(finished()), &m_CameraThread, SLOT(quit()), Qt::DirectConnection);

        m_CameraThread.start();

        //Start Auto Sync Thread;


        QThread *m_ASyncWorkerThread = new QThread();
        cAutoSync *m_ASyncWorker = new cAutoSync();
        m_ASyncWorker->moveToThread(m_ASyncWorkerThread);
        QObject::connect(m_ASyncWorkerThread, SIGNAL(started()), m_ASyncWorker, SLOT(mainLoop()));
        QObject::connect(m_ASyncWorker, SIGNAL(finished()), m_ASyncWorkerThread, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(m_ASyncWorker, SIGNAL(sigPostPicturesToServer(QList<cPicturesData>)), m_serverutils, SLOT(postPictureToServerFromThread(QList<cPicturesData>)));
        QObject::connect(m_ASyncWorker, SIGNAL(sigPostDataToServer(QList<cDataSession>)), m_serverutils, SLOT(postDataToServerFromThread(QList<cDataSession>)));
        m_ASyncWorkerThread->start();

        QTimer dateTimer;
        dateTimer.setInterval(5000);
        a.connect(&dateTimer, &QTimer::timeout, [m_serverutils] () {
            QString dateTime = m_serverutils->getDateTime();
            qDebug() << "Set Date Time every 5s: " << dateTime;
            system(QString("timedatectl set-time '%1'").arg(dateTime).toLatin1().data());
        });
        dateTimer.start();
        a.processEvents();
        MainWindow w;
        QObject::connect(m_ASyncWorker, SIGNAL(sigSyncDone()), &w, SLOT(onDatabaseSyncDone()));
        QObject::connect(m_ASyncWorker, SIGNAL(sigSyncing()), &w, SLOT(onDatabseSyncing()));
        QObject::connect(m_CameraWorker, SIGNAL(sigCameraFrameReady(QPixmap)), &w, SLOT(onFramedataReady(QPixmap)));
        w.setMaximumSize(QSize(800, 480));
#ifdef BUILD_PC
        w.showNormal();
#else
        w.showFullScreen();
#endif
//        splash.finish(&w);
        return a.exec();
    } else {
        qDebug() << "Database Init Error, cannot start application!";
    }
}
