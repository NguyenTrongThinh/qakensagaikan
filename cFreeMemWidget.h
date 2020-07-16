#ifndef CFREEMEMWIDGET_H
#define CFREEMEMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>
#include <QTimer>
#include <QHBoxLayout>

class cFreeMemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cFreeMemWidget(QWidget *parent = 0);
    ~cFreeMemWidget();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QTimer *m_Timer = nullptr;
    QLabel *m_MemLabel = nullptr;
    QHBoxLayout *m_HBoxLayout = nullptr;
signals:
    void sigMouseReleaseEvent();
    void sigLowMemoryEvent();
private slots:
    void onTimerTimeout();
};

#endif // CFREEMEMWIDGET_H
