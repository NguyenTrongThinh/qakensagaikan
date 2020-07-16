#ifndef CCAMERAVIEWER_H
#define CCAMERAVIEWER_H

#include <QObject>
#include <QLabel>
#include <QFocusEvent>

class cCameraViewer : public QLabel
{
    Q_OBJECT
public:
    cCameraViewer();
    ~cCameraViewer();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CCAMERAVIEWER_H
