#ifndef CCUSTOMLABEL_H
#define CCUSTOMLABEL_H
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>


class cSettingButton : public QLabel
{
    Q_OBJECT
public:
    explicit cSettingButton(QWidget *parent = nullptr);
    ~cSettingButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
 void sigMouseReleaseEvent();
};

#endif // CCUSTOMLABEL_H
