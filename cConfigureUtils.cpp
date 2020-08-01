#include "cConfigureUtils.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QVariantMap>
#include <QTextStream>
#include <QDebug>
#include <QProcess>

#define configureFileName  "kensabonder.cfg"


cConfigureUtils::cConfigureUtils(QObject *parent) : QObject(parent)
{

}

cConfigureUtils::~cConfigureUtils()
{

}

void cConfigureUtils::setServerAddress(QString serverAddress)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("server");
            settingContent.insert("server", serverAddress);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("server", serverAddress);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setScannerVID(QString vid)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("scannervid");
            settingContent.insert("scannervid", vid);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("scannervid", vid);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setScannerPID(QString pid)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("scannerpid");
            settingContent.insert("scannerpid", pid);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("scannerpid", pid);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setDelayValue(int sec)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("delay");
            settingContent.insert("delay", sec);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("delay", sec);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setPicturesDir(QString dir)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("picdir");
            settingContent.insert("picdir", dir);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("picdir", dir);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setManaualMode(QString mode)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("manmode");
            settingContent.insert("manmode", mode);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("manmode", mode);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setNumOfSameScan(int num)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("numofsamescan");
            settingContent.insert("numofsamescan", num);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("numofsamescan", num);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setPassword(QString password)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("password");
            settingContent.insert("password", password);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("password", password);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}

void cConfigureUtils::setLine(QString line)
{
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    QJsonDocument itemDoc;
    QVariantMap settingContent;
    if(cfgFile.open(QIODevice::ReadWrite)) {
        QTextStream in(&cfgFile);
        QString content = in.readAll();
        cfgFile.close();
        if (!content.isEmpty()) {
            itemDoc = QJsonDocument::fromJson(content.toLatin1());
            settingContent = itemDoc.object().toVariantMap();
            settingContent.remove("line");
            settingContent.insert("line", line);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        } else {
            settingContent.insert("line", line);
            itemDoc = QJsonDocument::fromVariant(settingContent);
            cfgFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            cfgFile.write(itemDoc.toJson());
            cfgFile.close();
        }
    }
}


QString cConfigureUtils::getServerAdddress()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("server").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getScannerVid()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);
    qDebug() << homeFolder + "/" + configureFileName;

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("scannervid").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getScannerPid()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("scannerpid").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getWifiMac()
{
    QString content = "";
    //Change to wlan0 for WIFI

#ifdef BUILD_PC
        QFile file("/sys/class/net/wlp5s0/address");
#else
        QFile file("/sys/class/net/wlan0/address");
#endif


    if (file.exists()) {
        if(file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            content = in.readAll();
            content.remove(":");
            content.remove("\n");
            file.close();
        }
    }
    return content;
}

QString cConfigureUtils::getEthernetMac()
{
    QString content = "";
    QFile file("/sys/class/net/eth0/address");
    if (file.exists()) {
        if(file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            content = in.readAll();
            content.remove(":");
            content.remove("\n");
            file.close();
        }
    }
    return content;
}

QString cConfigureUtils::getPicturesDir()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("picdir").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getManualMode()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("manmode").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getPassword()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("password").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getLine()
{
    QString retVal = "";
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("line").toString();
            }
        }
    }
    return retVal;
}

QString cConfigureUtils::getIpAddress()
{
    QProcess process;
    process.start("hostname -I");
    process.waitForFinished(-1); // will wait forever until finished
    QString stdout = process.readAllStandardOutput().trimmed();
    return stdout;
}

int cConfigureUtils::getDelayValue()
{
    int retVal = 0;
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("delay").toInt();
            }
        }
    }
    return retVal;
}

int cConfigureUtils::getNumOfSameScan()
{
    int retVal = 0;
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QFile cfgFile(homeFolder + "/" + configureFileName);

    if (cfgFile.exists()) {
        if(cfgFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&cfgFile);
            QString content = in.readAll();
            cfgFile.close();
            if (!content.isEmpty()) {
                QJsonDocument itemDoc = QJsonDocument::fromJson(content.toLatin1());
                QJsonObject itemObj = itemDoc.object();
                retVal = itemObj.take("numofsamescan").toInt();
            }
        }
    }
    return retVal;
}
