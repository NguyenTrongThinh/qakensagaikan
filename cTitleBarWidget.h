#ifndef CTITLEBARWIDGET_H
#define CTITLEBARWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <cCancelButton.h>
#include <QLabel>
#include <cClockWidget.h>
#include <cWifiWidget.h>
#include <cSyncingWidget.h>
#include "cFreeMemWidget.h"


class cTitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cTitleBarWidget(QWidget *parent = nullptr);
    ~cTitleBarWidget();
    void setTitle(QString title);
    void setSyncingState();
    void setSyncingDoneState();
    void setSyncingDoneStateWithWarn();
    void hideCancelButton();
    void showCancelButton();
    void SetCancelButtonIcon(QString normal, QString clicked);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QHBoxLayout *m_HBoxLayout = nullptr;
    QLabel *m_TitleLabel = nullptr;
    cCancelButton *m_CancalButton = nullptr;
    cWifiWidget *m_WifiWidget = nullptr;
    cClockWidget *m_ClockWidget = nullptr;
    cSyncingWidget *m_SyncingWidget = nullptr;
    cFreeMemWidget *m_FreeMemWidget = nullptr;
signals:
    void sigCancelButtonClicked();
    void sigClockWidgetClicked();
    void sigWifiWidgetClicked();
    void sigWifiDisconnected();
    void sigWifiConnected();

private slots:
};

#endif // CTITLEBARWIDGET_H
