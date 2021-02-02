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
    void setMAB1(QString maab1);
    void setMAB2(QString maab2);
    void setLine(QString line);
    void setDelayTiming(int sec);
    void cancelOperation();
    void createMCUActionBox(int numBox);
    void createMagicStatus();
    void createDongMocStatus();
    void setOperatorStatus(int numOfOperator, bool isMCUConnect);
    void setMagicStatus(bool status);
    void setDongMocStatus(bool status);
private:
    Ui::wCheckingKanban *ui;
    QTimer *m_Timer = nullptr;
    int m_Delay = 0;
    bool isFinishOperator = false;
signals:
    void sigOKCkicked();
    void sigNGClicked();
    void sigSettingClicked();
    void sigOperatorFinish();
    void sigMagicFinish();
    void sigDongmocFinish();

private slots:
    void onTimerTimeout();
    void onbtnNGClicked();
    void onbtnSettingClicked();

public slots:
    void onBtnOKClicked();

};

#endif // WCHECKINGKANBAN_H
