#include "cAutoSync.h"
#include <cSqliteDatabase.h>
#include <cDataSession.h>
#include <cServerUtils.h>
#include <QProcess>
#include <QString>

cAutoSync::cAutoSync(QObject *parent) : QObject(parent)
{
    m_Abort = false;
}

cAutoSync::~cAutoSync()
{
    abort();
}

void cAutoSync::abort()
{
    QMutexLocker locker(&m_Mutex);
    m_Abort = true;
}

QString cAutoSync::getBSSID()
{
    QString program = "iwgetid";
    QProcess m_Process;
    m_Process.start(program);
    m_Process.waitForFinished(2000); // sets current thread to sleep and waits for pingProcess end
    QString output(m_Process.readAllStandardOutput());
    if (!output.contains("ESSID")) {
        output = "không có wifi";
    }
    return output;
}

void cAutoSync::mainLoop()
{
    cSQliteDatabase *m_Database = cSQliteDatabase::instance();
    cServerUtils *m_ServerUtils = cServerUtils::instance();
    QList<cDataSession> m_UnsubmitedTransaction;
    QList<cPicturesData> m_UnsubmitedPicturesTransaction;
    forever {
        m_Mutex.lock();
        if (m_Abort)
        {
           emit finished();
           m_Mutex.unlock();
           return;
        }
        m_Mutex.unlock();
        if (QString::compare(getBSSID(), "không có wifi") != 0) {
            m_UnsubmitedTransaction = m_Database->getUnsubmitedTransaction();
            if (m_UnsubmitedTransaction.count() > 0) {
                emit sigSyncing();
                emit sigPostDataToServer(m_UnsubmitedTransaction);
                do {

                    QThread::msleep(1000);
                } while(!m_ServerUtils->getSyncDataStatus());
                emit sigSyncDone();
            }

            m_UnsubmitedPicturesTransaction = m_Database->getUnsubmitedPicturesTransaction();
            if (m_UnsubmitedPicturesTransaction.count() > 0) {
                emit sigSyncing();
                emit sigPostPicturesToServer(m_UnsubmitedPicturesTransaction);
                do {

                    QThread::msleep(500);
                } while(!m_ServerUtils->getSyncPicturesStatus());
                emit sigSyncDone();
            }          
        }
        QThread::msleep(1000);
    }
}
