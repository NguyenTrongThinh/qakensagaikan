#ifndef CWIFIWIDGET_H
#define CWIFIWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFocusEvent>
#include <QTimer>
#include <QProcess>

class cWifiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cWifiWidget(QWidget *parent = nullptr);
    ~cWifiWidget();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QHBoxLayout *m_HBoxLayout = nullptr;
    QLabel *m_IconLabel = nullptr;
    QTimer *m_Timer = nullptr;
    QProcess *m_Process = nullptr;

    QString getWifiBSSID();
    bool m_isWifiConnected = false;

    void setIconDisconnected();
    void setIconConnected();

signals:
    void sigWifiWidgetClicked();
    void sigWifiDisconnected();
    void sigWifiConnected();
private slots:
    void onTimerTicked();
};

#endif // CWIFIWIDGET_H
