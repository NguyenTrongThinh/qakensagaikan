#ifndef CCAMERAWORKER_H
#define CCAMERAWORKER_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QPixmap>
#include "cSqliteDatabase.h"
#include "cPicturesData.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class cCameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit cCameraWorker(QObject *parent = 0);
    ~cCameraWorker();
    cCameraWorker (const cCameraWorker &);
    cCameraWorker& operator=(const cCameraWorker & );
    enum {CHUPANH, LUUANH, STREAMANH, SLEEP};
    static cCameraWorker *instance();
    static void drop();
    void abort();
    void requestMethod(int m_method);
    void setKanbanCode(QString kanbanCode);
private:
    Mat captureImageFromCamera();
    bool saveImage(QString kanbanCode, Mat frame);
private:
    static cCameraWorker *m_Instance;
    QMutex m_Mutex;
    cSQliteDatabase *m_Database = nullptr;
    cPicturesData m_PictureData;
    bool m_Abort;
    int m_Method;
    VideoCapture m_Cap;
    Mat m_Frame;
    QPixmap m_QFrame;
    QString m_kanbanCode;
    QString m_lastFileName;
signals:
    void finished();
    void sigCameraFrameReady(QPixmap m_Pixmap);
    void sigSaveImageSuccess();
public slots:
    void mainLoop();
};

#endif // CCAMERAWORKER_H
