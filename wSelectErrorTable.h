#ifndef SELECTERRORTABLE_H
#define SELECTERRORTABLE_H

#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QSignalMapper>

namespace Ui {
class SelectErrorTable;
}

class wSelectErrorTable : public QWidget
{
    Q_OBJECT

public:
    explicit wSelectErrorTable(QWidget *parent = nullptr);
    ~wSelectErrorTable();
    void setErrorTableList(QStringList list);
    void createButtonTable();
    QString getSelectedTableName();
private:
    Ui::SelectErrorTable *ui;
    QStringList m_ErrorTableList;
    QPushButton *m_PushButton;
    QList<QPushButton*> m_ListButon;
    QSignalMapper *m_SigMapper;
    QString m_SelectedTableName;
signals:
    void sigErrorTableSetected(QString tableName);
private slots:
    void onButtonClick(qint32 index);

};

#endif // SELECTERRORTABLE_H
