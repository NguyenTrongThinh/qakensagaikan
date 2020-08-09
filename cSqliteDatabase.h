#ifndef CSQLITEDATABASE_H
#define CSQLITEDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPair>
#include "cDataSession.h"
#include "cDataSessionActivating.h"
#include "cPicturesData.h"
#include <QMutex>
#include <cdatamh.h>

class cSQliteDatabase : public QObject
{
    Q_OBJECT
public:
    explicit cSQliteDatabase(QObject *parent = nullptr);
    ~cSQliteDatabase();
    static const int SUBMITED = 1;
    static const int UNSUBMITED = 0;

    cSQliteDatabase (const cSQliteDatabase &);
    cSQliteDatabase& operator=(const cSQliteDatabase & );

    static cSQliteDatabase *instance();
    static void drop();

    QSqlError initDatabase();
    void deinitDatabase();
    bool createErrorTable(QString tableName);
    bool deleteErrorTable();
    bool insertAllIntoTable(QString tableName, QList<QPair<QString, QString>> tableContent);
    bool insertAllMHIntoTable(QList<cDataMH> tableMH);
    bool istableExist(QString tableName);
    QList<QPair<QString, QString>> getNewError();
    QList<cDataMH> getNewDataMH();
    QString getMCUAction();
    void clearDataMH();
    void clearNewError();
    bool createTransactionTable();
    bool insertHistoryTransaction(cDataSession dataSession, int submited);
    QList<cDataSession> getUnsubmitedTransaction();
    bool getOldMHTable();
    bool updateUnsubmitedTransaction(int submited, QString dateTime);
    bool deleteOldSubmitedTransaction();
    bool deleteOldMHTable();
    QStringList getErrorTableName();
    QList<QPair<QString, QString>> getErrorTablecontent(QString tableName);

    bool createTempSessionTable();
    bool createTempMHTable();
    bool insertTempSession(cDataSessionActivating activatingSession);
    bool appendTempPictureName(QStringList name);
    bool deleteTempSession();
    cDataSessionActivating getTempSession();

    bool createPicturesLocationTable();
    bool insertPicturesTransaction(cPicturesData picture, int submited);
    bool updateSubmitedPicturesTransaction(cPicturesData picture, int submited);
    bool deleteOldSubmitedPicturesTransaction();
    QList<cPicturesData> getUnsubmitedPicturesTransaction();
    QList<cPicturesData> getOldSubmitedPicturesTransaction();
private:
    static cSQliteDatabase *m_Instance;
    QSqlDatabase m_Database;
    QMutex m_Mutex;
    QList<QPair<QString, QString>> m_NewError;
    QList<cDataMH> m_DataMH;
    bool isErrorCodeExist(QString tableName, QString errorCode);
    bool insertIntoTable(QString tableName, QPair<QString, QString> tableContent);
    bool insertIntoMHTable(QString tableNam, cDataMH mhdata);
signals:

public slots:
};

#endif // CSQLITEDATABASE_H
