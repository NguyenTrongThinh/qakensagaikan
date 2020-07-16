#ifndef CCAMERACONTROLLERWIDG_H
#define CCAMERACONTROLLERWIDG_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "cTrashButton.h"
#include "cSaveButton.h"

class cCameraControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cCameraControllerWidget(QWidget *parent = 0);
    ~cCameraControllerWidget();
private:
    cSaveButton *m_SaveButton = nullptr;
    cTrashButton *m_TrashButton = nullptr;
    QVBoxLayout *m_VBoxLayout = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void sigTrashButtonClicked();
    void sigSaveButtonClicked();
private slots:
};

#endif // CCAMERACONTROLLERWIDG_H
