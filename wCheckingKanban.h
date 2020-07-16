#ifndef WCHECKINGKANBAN_H
#define WCHECKINGKANBAN_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class wCheckingKanban;
}

class wCheckingKanban : public QWidget
{
    Q_OBJECT

public:
    explicit wCheckingKanban(QWidget *parent = 0);
    ~wCheckingKanban();
    void setMNV(QString mnv);
    void setMKB(QString kanban);
    void setDelayTiming(int sec);
    void cancelOperation();
private:
    Ui::wCheckingKanban *ui;
    QTimer *m_Timer = nullptr;
    int m_Delay = 0;
signals:
    void sigOKCkicked();
    void sigNGClicked();
    void sigSettingClicked();

private slots:
    void onTimerTimeout();
    void onbtnNGClicked();
    void onbtnSettingClicked();

public slots:
    void onBtnOKClicked();

};

#endif // WCHECKINGKANBAN_H
