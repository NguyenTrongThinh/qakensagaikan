#ifndef WCAMERASTREAM_H
#define WCAMERASTREAM_H

#include <QWidget>
#include "cCameraViewer.h"
#include <QPixmap>
#include "cCameraControllerWidg.h"

namespace Ui {
class wCameraStream;
}

class wCameraStream : public QWidget
{
    Q_OBJECT

public:
    explicit wCameraStream(QWidget *parent = 0);
    ~wCameraStream();
    void setImage(QPixmap pixmap);
private:
    cCameraViewer *m_CameraViewer = nullptr;
    cCameraControllerWidget *m_CameraControllerWidget = nullptr;

private:
    Ui::wCameraStream *ui;
signals:
    void sigCaptureImage();
    void sigSaveImage();
    void sigStreamImage();
};

#endif // WCAMERASTREAM_H
