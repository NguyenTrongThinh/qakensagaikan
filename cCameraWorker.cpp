#include "cCameraWorker.h"
#include <QDebug>
#include <QImage>
#include "cConfigureUtils.h"
#include <QDateTime>
#include <QDir>

cCameraWorker *cCameraWorker::m_Instance = nullptr;

cCameraWorker::cCameraWorker(QObject *parent) : QObject(parent)
{
    m_Abort = false;
    m_Method = -1;
    m_Database = cSQliteDatabase::instance();
}

cCameraWorker::~cCameraWorker()
{
    abort();
}

cCameraWorker *cCameraWorker::instance()
{
    static QMutex mutex;
    if (m_Instance == nullptr) {
       mutex.lock();
       m_Instance = new cCameraWorker();
       mutex.unlock();
    }
    return m_Instance;
}

void cCameraWorker::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

void cCameraWorker::abort()
{
    QMutexLocker locker(&m_Mutex);
    m_Abort = true;
}

void cCameraWorker::requestMethod(int m_method)
{
    QMutexLocker locker(&m_Mutex);
    m_Method = m_method;
}

void cCameraWorker::setKanbanCode(QString kanbanCode)
{
    QMutexLocker locker(&m_Mutex);
    m_kanbanCode = kanbanCode;
}

Mat cCameraWorker::captureImageFromCamera()
{
    QMutexLocker locker(&m_Mutex);
    Mat frames;
    if (!m_Cap.isOpened()) {
       bool retVal = m_Cap.open(0);
       qDebug() << "Camera is opened with RetVal: " << retVal;
       if (retVal) {
           m_Cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
           m_Cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
           m_Cap >> frames;
       }
    } else {
        m_Cap >> frames;
        cvtColor(frames, frames, CV_BGR2RGB);
    }
    return frames.clone();
}

bool cCameraWorker::saveImage(QString kanbanCode, Mat frame)
{
    bool retVal = true;
    QString sDir = cConfigureUtils::getPicturesDir();
    QDir dir = QDir(sDir);
    if (!dir.exists())
        retVal = dir.mkpath(sDir);
    if (retVal) {
        QString filename = cConfigureUtils::getWifiMac() + "_" + kanbanCode + "_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss")  + ".jpg";
        filename = dir.absolutePath() + "/" + filename;
        Mat imageCV = frame;
        cvtColor(imageCV, imageCV, CV_RGB2BGR);
        retVal = imwrite(filename.toStdString(), imageCV);
        if (retVal)
            m_lastFileName = filename;
        else
            m_lastFileName = "";
    }
    return retVal;
}


void cCameraWorker::mainLoop()
{
    bool retVal = false;
    bool isCaptured = false;
    forever {
        m_Mutex.lock();
        if (m_Abort)
        {
           emit finished();
           m_Mutex.unlock();
           return;
        }
        m_Mutex.unlock();
        switch (m_Method) {
        case STREAMANH:
            isCaptured = false;
            m_Frame = captureImageFromCamera();
            m_QFrame = QPixmap::fromImage(QImage((unsigned char*) m_Frame.data, m_Frame.cols, m_Frame.rows, m_Frame.step, QImage::Format_RGB888));
            emit sigCameraFrameReady(m_QFrame);
            break;
        case CHUPANH:
            if (!isCaptured) {
                m_Frame = captureImageFromCamera();
                m_QFrame = QPixmap::fromImage(QImage((unsigned char*) m_Frame.data, m_Frame.cols, m_Frame.rows, m_Frame.step, QImage::Format_RGB888));
                emit sigCameraFrameReady(m_QFrame);
                m_Method = SLEEP;
                isCaptured = true;
            }
            break;
        case LUUANH:
            retVal = saveImage(m_kanbanCode, m_Frame);
            if (retVal)
            {
                m_PictureData.setLocationOnDisk(m_lastFileName);
                m_PictureData.setSaveDate(QDateTime::currentDateTime());
                m_Database->insertPicturesTransaction(m_PictureData, cSQliteDatabase::UNSUBMITED);
                QFileInfo file(m_lastFileName);
                m_Database->appendTempPictureName(QStringList(file.fileName()));
                emit sigSaveImageSuccess();
                m_Method = SLEEP;
                isCaptured = false;
            }
            break;
        default:
            break;
        }
        QThread::msleep(50);
    }
}
