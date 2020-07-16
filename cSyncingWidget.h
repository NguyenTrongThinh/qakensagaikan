#ifndef CSYNCINGWIDGET_H
#define CSYNCINGWIDGET_H

#include <QObject>
#include <QLabel>
#include <QMovie>

class cSyncingWidget : public QLabel
{
    Q_OBJECT
public:
    explicit cSyncingWidget(QWidget *parent = nullptr);
    ~cSyncingWidget();
    void setSyncingState();
    void setSyncDoneState();
    void setSyncDoneStateWithWarning();
private:
    QMovie *m_Movie = nullptr;
signals:

private slots:
};

#endif // CSYNCINGWIDGET_H
