#ifndef CMCUWIDGET_H
#define CMCUWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFocusEvent>
#include <QTimer>
#include <QProcess>

class cMCUWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cMCUWidget(QWidget *parent = nullptr);
    ~cMCUWidget();
    void setIconDisconnected();
    void setIconConnected();

private:
    QHBoxLayout *m_HBoxLayout = nullptr;
    QLabel *m_IconLabel = nullptr;
    QProcess *m_Process = nullptr;

    bool m_isMCUConnect = false;



};

#endif // CMCUWIDGET_H
