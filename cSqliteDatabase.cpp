#include "cSqliteDatabase.h"
#include <QStandardPaths>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QMutex>

#define dbname "kensa.sqlite"
#define transactiontable "history"
#define tempsession        "tempsession"
#define pictures            "pictures"
#define mhsession           "mahang"

cSQliteDatabase *cSQliteDatabase::m_Instance = nullptr;


cSQliteDatabase::cSQliteDatabase(QObject *parent) : QObject(parent)
{
    m_NewError.clear();
}

cSQliteDatabase::~cSQliteDatabase()
{

}

cSQliteDatabase *cSQliteDatabase::instance()
{
    static QMutex mutex;
    if (m_Instance == nullptr) {
       mutex.lock();
       m_Instance = new cSQliteDatabase();
       mutex.unlock();
    }
    return m_Instance;
}

void cSQliteDatabase::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

QSqlError cSQliteDatabase::initDatabase()
{
    QMutexLocker locker(&m_Mutex);
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    m_Database = QSqlDatabase::addDatabase("QSQLITE");
    m_Database.setDatabaseName(homeFolder + "/" + dbname);
    if (!m_Database.open())
        return m_Database.lastError();
    return QSqlError("", "", QSqlError::NoError);
}

void cSQliteDatabase::deinitDatabase()
{
    QMutexLocker locker(&m_Mutex);
    m_Database.close();
}

bool cSQliteDatabase::createErrorTable(QString tableName)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS \"%1\"(id INTEGER PRIMARY KEY AUTOINCREMENT, errorcode CHAR(5), errorstring TEXT)").arg(tableName);
    qDebug() << cmd;
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::deleteErrorTable()
{
    QStringList errorTableName = getErrorTableName();
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    foreach(QString tableName, errorTableName) {
        QString cmd = QString("DROP TABLE IF EXISTS \"%1\"").arg(tableName);
        query.prepare(cmd);
        query.exec();
        retVal = m_Database.transaction();
        if (retVal)
            retVal = m_Database.commit();
    }
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertAllIntoTable(QString tableName, QList<QPair<QString, QString> > tableContent)
{
    bool retVal = true;
    if (!istableExist(tableName))
        return retVal;
    for (int i = 0; i < tableContent.count(); i++) {
//        if (!isErrorCodeExist(tableName, tableContent.at(i).first)) {
//        if (i < 10) {
            retVal = insertIntoTable(tableName, tableContent.at(i));
            m_NewError.append(QPair<QString, QString>(tableContent.at(i).first, tableContent.at(i).second));
//        } else
//            break;

    }
    return retVal;
}

bool cSQliteDatabase::insertAllMHIntoTable(QList<cDataMH> tableMH)
{
    bool retVal = true;
    cDataMH data;
    if (!istableExist(mhsession))
        return retVal;
    for (int i = 0; i < tableMH.count(); i++) {
        data.setMaHang(tableMH[i].getMaHang());
        data.setSoMaAB(tableMH[i].getSoMaAB());
        data.setMaAB1(tableMH[i].getMaAB1());
        data.setMaAB2(tableMH[i].getMaAB2());
        data.setThaoTacMCU(tableMH[i].getThaoTacMCU());
        data.setChamMagic(tableMH[i].getChamMagic());
        data.setDongMoc(tableMH[i].getDongMoc());
        retVal = insertIntoMHTable(mhsession, data);
        m_DataMH.append(data);

    }
    return retVal;
}

bool cSQliteDatabase::istableExist(QString tableName)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.tables().contains(tableName))
        return retVal;
    return true;
}

QList<QPair<QString, QString> > cSQliteDatabase::getNewError()
{
    return m_NewError;
}

QList<cDataMH> cSQliteDatabase::getNewDataMH()
{
    return m_DataMH;
}

QString cSQliteDatabase::getMCUAction()
{

}

void cSQliteDatabase::clearDataMH()
{
    m_DataMH.clear();
}

void cSQliteDatabase::clearNewError()
{
    m_NewError.clear();
}

bool cSQliteDatabase::createTransactionTable()
{
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS \"%1\"(id INTEGER PRIMARY KEY AUTOINCREMENT, mnv TEXT, time TEXT,\
                 deviceid TEXT, line TEXT, makanban TEXT, ngayin TEXT, nameplate TEXT, maab1 TEXT, ngayinmaab1 TEXT,\
                 nameplateab1 TEXT, prefixab1 TEXT, maab2 TEXT, ngayinmaab2 TEXT, nameplateab2 TEXT, prefixab2 TEXT,\
                 loi TEXT, ca TEXT, hinh TEXT, submit INTEGER)").arg(transactiontable);
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertHistoryTransaction(cDataSession dataSession, int submited)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }

    QString mnv = dataSession.getmnv();
    QString time = dataSession.gettime();
    QString mahang = dataSession.getMHCode();
    QString dateprintMH = dataSession.getMHDatePrint();
    QString nameplateMH = dataSession.getMHNamePlate();
    QString maab1 = dataSession.getMaAB1();
    QString dateprintab1 = dataSession.getDatePrintAB1();
    QString nameplateab1 = dataSession.getNamePlateAB1();
    QString prefixab1 = dataSession.getPrefixAB1();
    QString maab2 = dataSession.getMaAB2();
    QString dateprintab2 = dataSession.getDatePrintAB2();
    QString nameplateab2 = dataSession.getNamePlateAB2();
    QString prefixab2 = dataSession.getPrefixAB2();
    QString line = dataSession.getLine();
    QString ca = dataSession.getca();
    QString deviceid = dataSession.getdeviceid();
    QList<QPair<QString, int>> errorList = dataSession.getloi();
    QStringList picturesList = dataSession.getHinh();
    QString loi;
    for (int i = 0; i < errorList.count(); i++) {
        loi.append(errorList.at(i).first + "," + QString::number(errorList.at(i).second));
        if (i < (errorList.count() - 1)) {
            loi.append(":");
        }
    }
    QString hinh;
    for (int  i = 0; i < picturesList.count(); i++) {
        hinh.append(picturesList.at(i));
        if (i < (picturesList.count() - 1))
            hinh.append(":");
    }

    QString cmd = QString("INSERT INTO history (mnv, time, deviceid, line, makanban, ngayin,\
             nameplate, maab1, ngayinmaab1, nameplateab1, prefixab1, maab2, ngayinmaab2,\
             nameplateab2, prefixab2, loi, ca, hinh, submit) \
             VALUES (:mnv, :time, :deviceid, :line, :makanban, :ngayin, :nameplate, :maab1, :ngayinmaab1, :nameplateab1, :prefixab1, :maab2, :ngayinmaab2, :nameplateab2, :prefixab2, :loi, :ca, :hinh, :submit)");
    QSqlQuery query = QSqlQuery(m_Database);
    query.prepare(cmd);
    query.bindValue(":mnv", mnv);
    query.bindValue(":time", time);
    query.bindValue(":deviceid", deviceid);
    query.bindValue(":line", line);
    query.bindValue(":makanban", mahang);
    query.bindValue(":ngayin", dateprintMH);
    query.bindValue(":nameplate", nameplateMH);
    query.bindValue(":maab1", maab1);
    query.bindValue(":ngayinmaab1", dateprintab1);
    query.bindValue(":nameplateab1", nameplateab1);
    query.bindValue(":prefixab1", prefixab1);
    query.bindValue(":maab2", maab2);
    query.bindValue(":ngayinmaab2", dateprintab2);
    query.bindValue(":nameplateab2", nameplateab2);
    query.bindValue(":prefixab2", prefixab2);
    query.bindValue(":loi", loi);
    query.bindValue(":ca", ca);
    query.bindValue(":hinh", hinh);
    query.bindValue(":submit", submited);
    qDebug() << "Command: " << cmd;

    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

QList<cDataSession> cSQliteDatabase::getUnsubmitedTransaction()
{
    QMutexLocker locker(&m_Mutex);
    QList<cDataSession> retVal;
    cDataSession datasession;
    QList<QPair<QString, int>> loi;
    retVal.clear();
    loi.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT * FROM \"%1\" WHERE submit = \"%2\"").arg(transactiontable).arg(cSQliteDatabase::UNSUBMITED);
    query.prepare(cmd);
    query.exec();
    while(query.next()) {
        datasession.setmnv(query.value(1).toString());
        datasession.settime(query.value(2).toString());
        datasession.setdeviceid(query.value(3).toString());
        datasession.setLine(query.value(4).toString());
        datasession.setMHCode(query.value(5).toString());
        datasession.setMHDatePrint(query.value(6).toString());
        datasession.setMHNamePlate(query.value(7).toString());
        datasession.setMaAB1(query.value(8).toString());
        datasession.setDatePrintAB1(query.value(9).toString());
        datasession.setNamePlateAB1(query.value(10).toString());
        datasession.setPrefixAB1(query.value(11).toString());
        datasession.setMaAB2(query.value(12).toString());
        datasession.setDatePrintAB2(query.value(13).toString());
        datasession.setNamePlateAB2(query.value(14).toString());
        datasession.setPrefixAB2(query.value(15).toString());
        if (!query.value(16).toString().isEmpty()) {
            QStringList loiList = query.value(16).toString().split(':');
            foreach(QString loiPair, loiList) {
                QStringList motloi = loiPair.split(',');
                loi.append(QPair<QString, int>(motloi.first(), motloi.last().toInt()));
            }
        }
        datasession.setloi(loi);
        datasession.setca(query.value(17).toString());
        QString hinh = query.value(18).toString();
        QStringList picturesList = hinh.split(":");
        datasession.setHinh(picturesList);
        retVal.append(datasession);
        loi.clear();
    }
    query.clear();
    return retVal;
}

bool cSQliteDatabase::getOldMHTable()
{
    QMutexLocker locker(&m_Mutex);
    cDataMH dataMH;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return false;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT * FROM \"%1\"").arg(mhsession);
    query.prepare(cmd);
    query.exec();
    while(query.next()) {
        dataMH.setMaHang(query.value(1).toString());
        dataMH.setSoMaAB(query.value(2).toInt());
        dataMH.setMaAB1(query.value(3).toString());
        dataMH.setMaAB2(query.value(4).toString());
        dataMH.setThaoTacMCU(query.value(5).toString());
        dataMH.setChamMagic(query.value(6).toString());
        dataMH.setDongMoc(query.value(7).toString());
        m_DataMH.append(dataMH);
    }
    query.clear();
    return true;
}

bool cSQliteDatabase::updateUnsubmitedTransaction(int submited, QString dateTime)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);

    QString cmd = QString("UPDATE \"%1\" SET submit = %2 WHERE time = \"%3\"").arg(transactiontable).arg(submited).arg(dateTime);
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::deleteOldSubmitedTransaction()
{
    QMutexLocker locker(&m_Mutex);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QStringList deleteDateTime;
    bool retVal = false;
    deleteDateTime.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);

    QString querycmd = QString("SELECT * FROM \"%1\" WHERE submit = \"%2\"").arg(transactiontable).arg(cSQliteDatabase::SUBMITED);
    m_Database.transaction();
    query.prepare(querycmd);
    query.exec();
    while(query.next()) {
        QString sDateTime = query.value(2).toString();
        QDateTime rowDatabaseTime = QDateTime::fromString(sDateTime, "ddMMyyhhmmss");
        rowDatabaseTime.setDate(QDate(rowDatabaseTime.date().year() + 100, rowDatabaseTime.date().month(), rowDatabaseTime.date().day()));
        qDebug() << "Date To " << rowDatabaseTime.daysTo(currentDateTime);
        if (rowDatabaseTime.daysTo(currentDateTime) >= 90) {
            deleteDateTime.append(sDateTime);
        }
    }
    query.clear();
    retVal = true;
    if (deleteDateTime.count() > 0) {
        QString delcmd = QString("DELETE FROM \"%1\" WHERE submit = 1 AND time = :time").arg(transactiontable);
        query.prepare(delcmd);
        foreach (QString deleteRow, deleteDateTime) {
            query.bindValue(":time", deleteRow);
            query.exec();
        }
        qDebug() << "Del Command: " << delcmd;
    }
    retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::deleteOldMHTable()
{
    QStringList errorTableName = getErrorTableName();
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    foreach(QString tableName, errorTableName) {
        QString cmd = QString("DROP TABLE IF EXISTS \"%1\"").arg(mhsession);
        query.prepare(cmd);
        query.exec();
        retVal = m_Database.transaction();
        if (retVal)
            retVal = m_Database.commit();
    }
    query.clear();
    return retVal;
}

QStringList cSQliteDatabase::getErrorTableName()
{
    QMutexLocker locker(&m_Mutex);
    QStringList errorTableList;
    errorTableList.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return errorTableList;
    }
    errorTableList = m_Database.tables();
    errorTableList.removeOne(transactiontable);
    errorTableList.removeOne("sqlite_sequence");
    errorTableList.removeOne(tempsession);
    errorTableList.removeOne(pictures);
    errorTableList.removeOne(mhsession);
    return errorTableList;
}

QList<QPair<QString, QString> > cSQliteDatabase::getErrorTablecontent(QString tableName)
{
    QMutexLocker locker(&m_Mutex);
    QList<QPair<QString, QString>> retVal;

    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }

    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT * FROM \"%1\"").arg(tableName);
    query.prepare(cmd);
    query.exec();
    while(query.next()) {
        QPair<QString, QString> element(query.value(1).toString(), query.value(2).toString());
        retVal.append(element);
    }
    query.clear();
    return retVal;
}

bool cSQliteDatabase::createTempSessionTable()
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS \"%1\"(id INTEGER PRIMARY KEY AUTOINCREMENT, mnv CHAR(12), makanban CHAR(17), tenbangloi TEXT, maab1 TEXT, maab2 TEXT, mcuAction TEXT, hinh TEXT)").arg(tempsession);
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::createTempMHTable()
{

    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS mahang (id INTEGER PRIMARY KEY AUTOINCREMENT, mahang TEXT, somaab INTEGER, maab1 TEXT, maab2 TEXT, thaotacmcu TEXT, chammagic TEXT, dongmoc TEXT)");
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertTempSession(cDataSessionActivating activatingSession)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    retVal = deleteTempSession();
    if (retVal) {
        QSqlQuery query = QSqlQuery(m_Database);
        QStringList hinhName = activatingSession.getPicturesList();
        QString sqliteHinhList;
        for (int i = 0; i < hinhName.count(); i++) {
            sqliteHinhList.append(hinhName.at(i));
            if (i < (hinhName.count() - 1)) {
                sqliteHinhList.append(":");
            }
        }
        QString cmd = QString("INSERT INTO \"%1\" (mnv, makanban, tenbangloi, maab1, maab2, mcuAction, hinh) VALUES (\"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\")")
                .arg(tempsession)
                .arg(activatingSession.getMNV())
                .arg(activatingSession.getMaKanban())
                .arg(activatingSession.getTenBangLoi())
                .arg(activatingSession.getMaAB1())
                .arg(activatingSession.getMaAB2())
                .arg(activatingSession.getMCUAction())
                .arg(sqliteHinhList);
        query.prepare(cmd);
        query.exec();
        retVal = m_Database.transaction();
        if (retVal)
            retVal = m_Database.commit();
        query.clear();
    }
    return retVal;
}

bool cSQliteDatabase::appendTempPictureName(QStringList name)
{
    bool retVal = false;
    cDataSessionActivating cTempDataSession = getTempSession();
    QStringList pictureList = cTempDataSession.getPicturesList();
    pictureList.append(name);
    cTempDataSession.setPicturesList(pictureList);
    retVal = insertTempSession(cTempDataSession);
    return retVal;
}


bool cSQliteDatabase::deleteTempSession()
{
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);


    QString delcmd = QString("DELETE FROM \"%1\"").arg(tempsession);
    query.prepare(delcmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

cDataSessionActivating cSQliteDatabase::getTempSession()
{
    QMutexLocker locker(&m_Mutex);
    cDataSessionActivating retVal;
    QStringList pictureList;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT * FROM \"%1\"").arg(tempsession);
    query.prepare(cmd);
    query.exec();
    while(query.next()) {
        retVal.setMNV(query.value(1).toString());
        retVal.setMaKanban(query.value(2).toString());
        retVal.setTenBangLoi(query.value(3).toString());
        retVal.setMaAB1(query.value(4).toString());
        retVal.setMaAB2(query.value(5).toString());
        retVal.setMCUAction(query.value(6).toString());
        if (query.value(7).toString().isEmpty())
            pictureList.clear();
        else
            pictureList = query.value(6).toString().split(":");
        retVal.setPicturesList(pictureList);
        break;
    }
    query.clear();
    return retVal;
}

bool cSQliteDatabase::createPicturesLocationTable()
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS \"%1\"(id INTEGER PRIMARY KEY AUTOINCREMENT, location TEXT, date DATETIME, submit INTEGER)").arg(pictures);
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertPicturesTransaction(cPicturesData picture, int submited)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString location = picture.getLocationOnDisk();
    QString saveDate = picture.getSaveDate().toString("yyyy-MM-dd hh:mm:ss");

    QString cmd = QString("INSERT INTO \"%1\" (location, date, submit) VALUES (\"%2\", \"%3\", \"%4\")")
            .arg(pictures)
            .arg(location)
            .arg(saveDate)
            .arg(submited);

    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::updateSubmitedPicturesTransaction(cPicturesData picture, int submited)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);

    QString cmd = QString("UPDATE \"%1\" SET submit = %2 WHERE date = \"%3\"").arg(pictures).arg(submited).arg(picture.getSaveDate().toString("yyyy-MM-dd hh:mm:ss"));
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::deleteOldSubmitedPicturesTransaction()
{
    QMutexLocker locker(&m_Mutex);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QStringList deleteDateTime;
    bool retVal = false;
    deleteDateTime.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    m_Database.transaction();
    QSqlQuery query = QSqlQuery(m_Database);

    QString querycmd = QString("SELECT * FROM \"%1\" WHERE submit = \"%2\"").arg(pictures).arg(cSQliteDatabase::SUBMITED);

    query.prepare(querycmd);
    query.exec();
    while(query.next()) {
        QString sDateTime = query.value(2).toString();
        QDateTime rowDatabaseTime = QDateTime::fromString(sDateTime, "yyyy-MM-dd hh:mm:ss");
        //qDebug() << "Date To " << rowDatabaseTime.daysTo(currentDateTime);
        if (rowDatabaseTime.daysTo(currentDateTime) >= 90) {
            deleteDateTime.append(sDateTime);
        }
    }
    query.clear();
    retVal = true;
    if (deleteDateTime.count() > 0) {
        QString delcmd = QString("DELETE FROM \"%1\" WHERE submit = 1 AND date = :date").arg(pictures);
        query.prepare(delcmd);
        foreach (QString deleteRow, deleteDateTime) {
            query.bindValue(":date", deleteRow);
            query.exec();
        }
        qDebug() << "DEl Picture command: " << delcmd;
    }
    retVal = m_Database.commit();
    query.clear();
    return retVal;
}

QList<cPicturesData> cSQliteDatabase::getUnsubmitedPicturesTransaction()
{
    QMutexLocker locker(&m_Mutex);
    QList<cPicturesData> retVal;
    cPicturesData datasession;
    QList<QPair<QString, int>> loi;
    retVal.clear();
    loi.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT * FROM \"%1\" WHERE submit = \"%2\"").arg(pictures).arg(cSQliteDatabase::UNSUBMITED);
    query.prepare(cmd);
    query.exec();
    while(query.next()) {
        datasession.setLocationOnDisk(query.value(1).toString());
        datasession.setSaveDate(QDateTime::fromString(query.value(2).toString(), "yyyy-MM-dd hh:mm:ss"));
        retVal.append(datasession);
        loi.clear();
    }
    query.clear();
    return retVal;
}

QList<cPicturesData> cSQliteDatabase::getOldSubmitedPicturesTransaction()
{
    QMutexLocker locker(&m_Mutex);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QList<cPicturesData> retVal;
    retVal.clear();
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);

    QString querycmd = QString("SELECT * FROM \"%1\" WHERE submit = \"%2\"").arg(pictures).arg(cSQliteDatabase::SUBMITED);

    query.prepare(querycmd);
    query.exec();
    while(query.next()) {
        QString sDateTime = query.value(2).toString();
        QDateTime rowDatabaseTime = QDateTime::fromString(sDateTime, "yyyy-MM-dd hh:mm:ss");
        qDebug() << "Date To " << rowDatabaseTime.daysTo(currentDateTime);
        if (rowDatabaseTime.daysTo(currentDateTime) >= 180) {
            cPicturesData onePictireData;
            onePictireData.setLocationOnDisk(query.value(1).toString());
            onePictireData.setSaveDate(QDateTime::fromString(query.value(2).toString(), "yyyy-MM-dd hh:mm:ss"));
            retVal.append(onePictireData);
        }
    }
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertIntoTable(QString tableName, QPair<QString, QString> tableContent)
{
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("INSERT INTO \"%1\" (errorcode, errorstring) VALUES (\"%2\", \"%3\")").arg(tableName).arg(tableContent.first).arg(tableContent.second);
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::insertIntoMHTable(QString tableNam, cDataMH mhdata)
{
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("INSERT INTO \"%1\" (mahang, somaab, maab1, maab2, thaotacmcu, chammagic, dongmoc) VALUES (\"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\")").arg(tableNam).arg(mhdata.getMaHang()).arg(mhdata.getSoMaAB()).arg(mhdata.getMaAB1()).arg(mhdata.getMaAB2()).arg(mhdata.getThaoTacMCU()).arg(mhdata.getChamMagic()).arg(mhdata.getDongMoc());
    qDebug() << cmd;
    query.prepare(cmd);
    query.exec();
    retVal = m_Database.transaction();
    if (retVal)
        retVal = m_Database.commit();
    query.clear();
    return retVal;
}

bool cSQliteDatabase::isErrorCodeExist(QString tableName, QString errorCode)
{
    QMutexLocker locker(&m_Mutex);
    bool retVal = false;
    if (!m_Database.isOpen())
    {
        QSqlError error = initDatabase();
        if (error.type() != QSqlError::NoError)
            return retVal;
    }
    QSqlQuery query = QSqlQuery(m_Database);
    QString cmd = QString("SELECT errorcode FROM \"%1\" WHERE errorcode = \"%2\"").arg(tableName).arg(errorCode);
    query.prepare(cmd);
    query.exec();
    retVal = query.next();
    query.clear();
    return retVal;
}

