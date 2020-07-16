#ifndef CAUTOSYNC_H
#define CAUTOSYNC_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QList>
#include <cDataSession.h>
#include "cPicturesData.h"
#include "cServerUtils.h"

class cAutoSync : public QObject
{
    Q_OBJECT
public:
    explicit cAutoSync(QObject *parent = nullptr);
    ~cAutoSync();
    void abort();
private:
    QMutex m_Mutex;
    bool m_Abort;
    QString getBSSID();
signals:
    void finished();
    void sigPostDataToServer(const QList<cDataSession> &dataSession);
    void sigPostPicturesToServer(const QList<cPicturesData> &pictureDate);
    void sigSyncing();
    void sigSyncDone();
public slots:
    void mainLoop();
};

#endif // CAUTOSYNC_H
