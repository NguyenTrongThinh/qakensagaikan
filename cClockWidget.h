#ifndef CCLOCKWIDGET_H
#define CCLOCKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QFocusEvent>

class cClockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cClockWidget(QWidget *parent = nullptr);
    ~cClockWidget();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QTimer *m_Timer = nullptr;
    QLabel *m_Time = nullptr;
    QLabel *m_Date = nullptr;
    QVBoxLayout *m_VBoxLayout = nullptr;

signals:
    void onClockWidgetClicked();
private slots:
    void onTimerTicked();
};

#endif // CCLOCKWIDGET_H
