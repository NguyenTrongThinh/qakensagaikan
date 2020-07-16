#include "cServerUtils.h"
#include <QMutex>
#include <QTimer>
#include <QEventLoop>
#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QPair>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QHostAddress>
#include <QNetworkInterface>

cServerUtils *cServerUtils::m_Instance = nullptr;


cServerUtils::cServerUtils(QObject *parent) : QObject(parent)
{
    m_Database = cSQliteDatabase::instance();
    m_NetworkAccessManager = new QNetworkAccessManager();
    m_ServerAddress = "";
    m_SyncPicturesDone = false;
    m_SyncDataDone = false;
}

cServerUtils::~cServerUtils()
{

}

cServerUtils *cServerUtils::instance()
{
    static QMutex mutex;
    if (m_Instance == nullptr) {
        mutex.lock();
        m_Instance = new cServerUtils();
        mutex.unlock();
    }
    return m_Instance;
}

void cServerUtils::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

QJsonArray cServerUtils::getErrorTable()
{
    QNetworkRequest request;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;
    QJsonArray retVal;
    timer.setSingleShot(true);
    request.setUrl(QUrl(m_ServerAddress + "/bangloi"));
    reply = m_NetworkAccessManager->get(request);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(10000);
    loop.exec();
    if(timer.isActive()) {
        timer.stop();
        if(reply->error() > 0) {
          qDebug() << "Error: " << reply->error() << "text: " << reply->errorString();
        }
        else {
          int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
          qDebug() << "Http Status Code: " << v;
          if (v >= 200 && v < 300) {
            QByteArray readData = reply->readAll();
            qDebug() << "Error Table: " << readData;
            QJsonDocument itemDoc = QJsonDocument::fromJson(readData);
            if (itemDoc.isArray()) {
                QJsonArray rootArray = itemDoc.array();
                retVal = rootArray;
            }
          }
        }
    } else {
       disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
       reply->abort();
    }
    delete reply;
    return retVal;
}

QJsonArray cServerUtils::getMHTable()
{
    QNetworkRequest request;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;
    QJsonArray retVal;
    QString header;
    timer.setSingleShot(true);
    request.setUrl(QUrl(m_ServerAddress + "/mahang"));
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            qDebug() << address.toString();
            header  = address.toString();
        }

    }

    request.setRawHeader(QByteArray("deviceid"), header.toUtf8());

    reply = m_NetworkAccessManager->get(request);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(10000);
    loop.exec();
    if(timer.isActive()) {
        timer.stop();
        if(reply->error() > 0) {
          qDebug() << "Error: " << reply->error() << "text: " << reply->errorString();
        }
        else {
          int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
          qDebug() << "Http Status Code: " << v;
          if (v >= 200 && v < 300) {
            QByteArray readData = reply->readAll();
            qDebug() << "MH Table: " << readData;
            QJsonDocument itemDoc = QJsonDocument::fromJson(readData);
            if (itemDoc.isArray()) {
                QJsonArray rootArray = itemDoc.array();
                retVal = rootArray;
            }
          }
        }
    } else {
       disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
       reply->abort();
    }
    delete reply;
    return retVal;
}

QStringList cServerUtils::getListTableName(QJsonArray tableArray)
{
    QStringList retVal;
    retVal.clear();
    for(int i=0; i < tableArray.count(); i++){
       QJsonValue value = tableArray.at(i);
       QVariantMap table = value.toObject().toVariantMap();
       QString tableName = table.value("tenbang").toString();

       if (!tableName.isEmpty())
           retVal.append(tableName);
    }
    return retVal;
}

QList<cDataMH> cServerUtils::getListDataMH(QJsonArray dataArray)
{
    QList<cDataMH> listMH;
    cDataMH dataMH;
    listMH.clear();
    for(int i=0; i < dataArray.count(); i++){
       QJsonValue value = dataArray.at(i);
       QVariantMap mahang = value.toObject().toVariantMap();
       dataMH.setMaHang(mahang.value("mahang").toString());
       dataMH.setSoMaAB(mahang.value("somaab").toInt());
       qDebug() << "List ma AB" << mahang.value("maab").toJsonArray();
       dataMH.setMaAB1(mahang.value("maab").toJsonArray().at(0).toString());
       dataMH.setMaAB2(mahang.value("maab").toJsonArray().at(1).toString());
       qDebug() << "Ma AB 1" << mahang.value("maab").toJsonArray().at(0).toString();
       qDebug() << "Ma AB 2" << mahang.value("maab").toJsonArray().at(1).toString();
       dataMH.setThaoTacMCU(mahang.value("thaotacmcu").toString());
       dataMH.setChamMagic(mahang.value("chammagic").toString());
       dataMH.setDongMoc(mahang.value("dongmoc").toString());
       listMH.append(dataMH);
    }
    return listMH;
}

QList<QPair<QString, QString> > cServerUtils::getListTablecontent(QJsonArray tableArray, QString tableName)
{
    QList<QPair<QString, QString> > retval;
    for(int i=0; i < tableArray.count(); i++){
       QJsonValue value = tableArray.at(i);
       QVariantMap table = value.toObject().toVariantMap();
       QString localtableName = table.value("tenbang").toString();

       if (QString::compare(localtableName, tableName) == 0) {
           QVariantList contentarray = table.value("noidung").toList();
           for(int j=0; j < contentarray.count(); j++){
              QVariantMap row = contentarray.at(j).toMap();
              QString text = row.value("tenloi").toString();
              QString errorcode = row.value("maloi").toString();
              QPair<QString, QString> rowPair(errorcode, text);
              retval.append(rowPair);
           }
           break;
       }
    }
    return retval;
}

int cServerUtils::postDataToServer(const cDataSession &dataSession)
{
    //QMutexLocker locker(&m_Mutex);
    int retVal = 400;
    QTimer timer;
    QEventLoop loop;
    QNetworkRequest request;
    request.setUrl(QUrl(m_ServerAddress));
    request.setRawHeader("Content-Type", "application/json");
    QVariantMap sessionData;
    sessionData.insert("mnv", dataSession.getmnv());
    sessionData.insert("time", dataSession.gettime());
    sessionData.insert("seibango", dataSession.getseibango());
    sessionData.insert("deviceid", dataSession.getdeviceid());
    sessionData.insert("soto", dataSession.getsoto());
    sessionData.insert("sosoi", dataSession.getsosoi());
    sessionData.insert("block", dataSession.getblock());
    sessionData.insert("ca", dataSession.getca());
    sessionData.insert("hinh", dataSession.getHinh());
    QVariantList loiList;
    QVariantMap loi;
    loiList.clear();
    for (int i = 0; i < dataSession.getloi().count(); i++) {
        QPair<QString, int> element = dataSession.getloi().at(i);
        loi.insert("maloi", element.first);
        loi.insert("soluongloi", element.second);
        loiList.append(loi);
    }
    sessionData.insert("loi", loiList);

    QJsonDocument itemdoc = QJsonDocument::fromVariant(sessionData);

    QNetworkReply *reply = m_NetworkAccessManager->post(request, itemdoc.toJson());


    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(10000);
    loop.exec();
    if(timer.isActive()) {
        timer.stop();
        if(reply->error() > 0) {
          qDebug() << "Error: " << reply->error() << "text: " << reply->errorString();
        }
        else {
          int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
          qDebug() << "Http Status Code: " << v;
          retVal = v;
          if (v >= 200 && v < 300) {
            QByteArray readData = reply->readAll();
            qDebug() << "Http Status code: " << readData;
          }
        }
    } else {
       disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
       reply->abort();
    }
    delete reply;
    return retVal;
}

int cServerUtils::postPictureToServer(const cPicturesData &dataSession)
{
    int retVal = 400;
    QTimer timer;
    QEventLoop loop;
    QString filePath = dataSession.getLocationOnDisk();

    qDebug() << "File Path: " << filePath;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart contentType;
    contentType.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data"));

    QHttpPart imagePart;

    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"filename\"; filename=\"%1\";").arg(filePath)));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    QFile *file = new QFile(filePath);
    if (file->exists()) {
        if (file->open(QIODevice::ReadOnly)) {
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(contentType);
            multiPart->append(imagePart);

            QNetworkRequest request;
            request.setUrl(QUrl(m_ServerAddress + "/uploadimage"));
            QNetworkReply *reply = m_NetworkAccessManager->post(request, multiPart);
            multiPart->setParent(reply);

            connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            timer.start(10000);
            loop.exec();
            if(timer.isActive()) {
                timer.stop();
                if(reply->error() > 0) {
                  qDebug() << "Error: " << reply->error() << "text: " << reply->errorString();
                }
                else {
                  int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                  qDebug() << "Http Status Code: " << v;
                  retVal = v;
                  if (v >= 200 && v < 300) {
                    QByteArray readData = reply->readAll();
                    qDebug() << "Http Status code: " << readData;
                  }
                }
            } else {
               disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
               reply->abort();
            }
            delete reply;
        }
    }
    return retVal;
}

void cServerUtils::setServerAddress(QString addr)
{
    m_ServerAddress = addr;
}

QString cServerUtils::getServerAddress()
{
    return m_ServerAddress;
}

bool cServerUtils::getSyncPicturesStatus()
{
    return m_SyncPicturesDone;
}

bool cServerUtils::getSyncDataStatus()
{
    return m_SyncDataDone;
}

void cServerUtils::postDataToServerFromThread(const QList<cDataSession> &dataSession)
{
    m_SyncDataDone = false;
    for(int i = 0; i < dataSession.count(); i++) {
        qDebug() << "cServerUtils: " << QDateTime::currentDateTime().toString() << " : Start post Data To Server: " << dataSession.at(i).gettime();
        int retVal = postDataToServer(dataSession.at(i));
        if (retVal >= 200 && retVal < 300) {
            qDebug() << "cServerUtils: " << QDateTime::currentDateTime().toString() << " : Post to server Success" << dataSession.at(i).gettime();
            m_Database->updateUnsubmitedTransaction(cSQliteDatabase::SUBMITED, dataSession.at(i).gettime());
        }
    }
    m_SyncDataDone = true;
}

void cServerUtils::postPictureToServerFromThread(const QList<cPicturesData> &dataSession)
{
    m_SyncPicturesDone = false;
    for(int i = 0; i < dataSession.count(); i++) {
        qDebug() << "cServerUtils: " << QDateTime::currentDateTime().toString() << " : Start Post Picture to server: " << dataSession.at(i).getSaveDate();
        int retVal = postPictureToServer(dataSession.at(i));
        if (retVal >= 200 && retVal < 300) {
            qDebug() << "cServerUtils: " << QDateTime::currentDateTime().toString() << " : Post To server Success: " << dataSession.at(i).getSaveDate();
            m_Database->updateSubmitedPicturesTransaction(dataSession.at(i), cSQliteDatabase::SUBMITED);
        }
    }
    m_SyncPicturesDone = true;
}
