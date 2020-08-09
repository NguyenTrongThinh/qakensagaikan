#ifndef CSERVERUTILS_H
#define CSERVERUTILS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <cDataSession.h>
#include <cSqliteDatabase.h>
#include <QMutex>
#include "cPicturesData.h"
#include <cdatamh.h>

class cServerUtils : public QObject
{
    Q_OBJECT
public:
    explicit cServerUtils(QObject *parent = nullptr);
    ~cServerUtils();

    cServerUtils (const cServerUtils &);
    cServerUtils& operator=(const cServerUtils & );

    static cServerUtils *instance();
    static void drop();

    QJsonArray getErrorTable();
    QJsonArray getMHTable();
    QStringList getListTableName(QJsonArray tableArray);
    QList<cDataMH> getListDataMH(QJsonArray dataArray);

    QList<QPair<QString, QString>> getListTablecontent(QJsonArray tableArray, QString tableName);
    int postDataToServer(const cDataSession &dataSession);
    int postPictureToServer(const cPicturesData &dataSession);
    QString getDateTime();
    void setServerAddress(QString addr);
    QString getServerAddress();
    bool getSyncPicturesStatus();
    bool getSyncDataStatus();

private:
    static cServerUtils *m_Instance;
    QString m_ServerAddress;
    QNetworkAccessManager *m_NetworkAccessManager; 
    cSQliteDatabase *m_Database;
    QMutex m_Mutex;
    bool m_SyncPicturesDone;
    bool m_SyncDataDone;
signals:

public slots:
    void postDataToServerFromThread(const QList<cDataSession> &dataSession);
    void postPictureToServerFromThread(const QList<cPicturesData> &dataSession);
};

#endif // CSERVERUTILS_H
