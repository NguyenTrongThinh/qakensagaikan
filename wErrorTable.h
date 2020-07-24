#ifndef ERRORTABLE_H
#define ERRORTABLE_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QList>
#include <QSignalMapper>
#include <QPushButton>
#include <cRowErrorWidget.h>
#include <cNumberPad.h>
#include <cDataSession.h>
#include <cSqliteDatabase.h>
#include <QTimer>

namespace Ui {
class ErrorTable;
}

class wErrorTable : public QWidget
{
    Q_OBJECT

public:
    explicit wErrorTable(QWidget *parent = nullptr);
    ~wErrorTable();
    void setErrotableName(QString name);
    void clearCheckedItems();
    void setMNV(QString text);
    void setMH(QString text);
    void setAB1(QString ab1);
    void setAB2(QString ab2);
    void createErrorTableContent(QList<QPair<QString, QString>> content);
    void clearErrorTableContent();
private:
    Ui::ErrorTable *ui;
    QList<QPair<QString, QString>> m_tableContent;
    QSignalMapper *m_SignalMapper = nullptr;
    QList<cRowErrorWidget*> m_TableRowWidgetList;
    cRowErrorWidget *m_TableRowWidget;


    QString m_MNV;
    QString m_MH;
    QString m_AB1;
    QString m_AB2;

    cNumberPad *m_NumberPad;


    QPushButton *m_CameraButton;


    qint32 m_CurrentcheckBox = -1;

    cSQliteDatabase *m_DataBase;
signals:
    void finished();
    void sigCameraButtonClicked();
private slots:
    void onCheckBoxChecked(qint32 index);
    void onButtonClicked(QString label);
};

#endif // ERRORTABLE_H
