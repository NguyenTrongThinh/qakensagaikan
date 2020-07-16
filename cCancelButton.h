#ifndef CCANCELBUTTON_H
#define CCANCELBUTTON_H
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QFocusEvent>


class cCancelButton : public QLabel
{
    Q_OBJECT
public:
    explicit cCancelButton(QWidget *parent = nullptr);
    ~cCancelButton();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setICon(QString normal, QString clicked);
private:
    QString m_IconNornal;
    QString m_IconClicked;
signals:
 void sigMouseReleaseEvent();
};

#endif // CCANCELBUTTON_H
