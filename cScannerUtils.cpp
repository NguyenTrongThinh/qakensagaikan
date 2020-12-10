#include "cScannerUtils.h"
#include <QDebug>
#include <QString>
#include <cConfigureUtils.h>
#include <libse660/inc/se660.h>
#include <libse660/inc/se660_detector_helper.h>

#define DEF_SCANNER_PID									"2218"
#define DEF_SCANNER_VID 								"05f9"



cScannerUtils *cScannerUtils::m_Instance = nullptr;


cScannerUtils::cScannerUtils(QObject *parent) : QObject(parent)
{
    m_Abort = false;
}

cScannerUtils::~cScannerUtils()
{
	abort();
}

void cScannerUtils::abort()
{
    QMutexLocker locker(&m_Mutex);
    m_Abort = true;
}

cScannerUtils *cScannerUtils::instance()
{
    static QMutex mutex;
   if (m_Instance == nullptr) {
       mutex.lock();
       m_Instance = new cScannerUtils();
       mutex.unlock();
   }
   return m_Instance;
}

void cScannerUtils::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

void cScannerUtils::mainLoop()
{

    char scanner_name[255];
    int fd = -1;                  /* file descriptor for the scanner device */
    int bytes_read = 0;           /* number of bytes read by the read function */
    char scanner_in[30];          /* set this bigger if scanned input is more than 30 characters */
    char vid[5];
    char pid[5];
    QString ScannerPID, ScannerVID;
    ScannerVID = cConfigureUtils::getScannerVid();
    ScannerPID = cConfigureUtils::getScannerPid();
    if (!ScannerPID.isEmpty()) {
        strncpy(pid, ScannerPID.toLatin1().data(), sizeof (pid));
    } else {
        strncpy(pid, DEF_SCANNER_PID, sizeof (pid));
    }

    if (!ScannerVID.isEmpty()) {
        strncpy(vid, ScannerVID.toLatin1().data(), sizeof(vid));
    } else {
        strncpy(vid, DEF_SCANNER_VID, sizeof(vid));
    }
    setScannerPID(pid);
    setScannerVID(vid);
    if (createScannerDetectorHelper() != 0) {
        return;
    }

    if (startDetectScanner(scanner_name) == 0) {
        qDebug() << "cScannerUtils::mainLoop-Scanner Name: " << scanner_name;
    } else {
        qDebug() << "cScannerUtils::mainLoop-Error While Detecting Scanner";
    }

    releaseScannerDetectorHelper();

    if ((fd = openScannerDevice(scanner_name)) < 0)
    {
      qDebug()<< "cScannerUtils::mainLoop-Error While Opeing Scanner";
      return;
    }

    forever {
        m_Mutex.lock();
        if (m_Abort)
        {
           emit finished();
           m_Mutex.unlock();
           closeScannerDevice();
           return;
        }
        m_Mutex.unlock();
        bytes_read = startReadBlockingMode(scanner_in);

        if (bytes_read < 0) {
            closeScannerDevice();
            if (createScannerDetectorHelper() != 0) {
                    return;
            }
            if (startDetectScanner(scanner_name) == 0) {
                qDebug() << "cScannerUtils::mainLoop-Scanner Name: " << scanner_name;
            } else {
                qDebug() << "cScannerUtils::mainLoop-Error While Detecting Scanner";
            }

            releaseScannerDetectorHelper();
            if ((fd = openScannerDevice(scanner_name)) < 0)
              {
                  qDebug()<< "cScannerUtils::mainLoop-Error While Opeing Scanner";
                  return;
              }

        } else  {
            qDebug() << "cScannerUtils::mainLoop-SCANNER RETURN: " << scanner_in;
            emit onDataDetected(QString(scanner_in));
        }
    }
}
