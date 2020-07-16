#ifndef CNUMBERPAD2_H
#define CNUMBERPAD2_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QSignalMapper>
#include <QGridLayout>

class cNumberPad2 : public QWidget
{
    Q_OBJECT
public:
    explicit cNumberPad2(QWidget *parent = nullptr);
    ~cNumberPad2();
private:
    QPushButton *m_PushButton;
    QList<QPushButton*> m_ListButton;
    QList<QString> m_ButtonLabel;
    QSignalMapper *m_signalMapperKey;
    QGridLayout *m_GridLayout;
signals:
    void buttonClicked(QString keyLabel);
public slots:
    void onButtonClicked(qint32 index);
};

#endif // CNUMBERPAD2_H
