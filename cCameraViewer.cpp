#include "cCameraViewer.h"

cCameraViewer::cCameraViewer()
{

}

cCameraViewer::~cCameraViewer()
{

}

void cCameraViewer::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QLabel::mouseReleaseEvent(event);
}

void cCameraViewer::mousePressEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
}
