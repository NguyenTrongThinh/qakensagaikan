#ifndef ROWERRORWIDGET_H
#define ROWERRORWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <cCustomLineEdit.h>

class cRowErrorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit cRowErrorWidget(QWidget *parent = nullptr);
    ~cRowErrorWidget();
    void setLineEditText(const QString &text);
    void setCheckBoxText(const QString &text);
    bool isCheckBoxChecked();
    QString getLineEditText();
    void showLineEdit();
    void setCheckBoxFont(const QFont &font);
    void setLineEditFont(const QFont &font);
    void hideLineEdit();
    void setCheckBoxSize(QSize size);
    void clearCheckBoxChecked();
private:
    QCheckBox *m_CheckBox = nullptr;
    cCustomLineEdit *m_LineEdit =  nullptr;
    QHBoxLayout *m_HBoxLayout = nullptr;

signals:
    void clicked();
public slots:
    void onCheckBoxStateChanged(int status);
    void onLineEditClicked();
    void onLineEdirFocusInEvent();
    void onLineEditFocusOutEvent();
};

#endif // ROWERRORWIDGET_H
