#ifndef CCONFIGUREUTILS_H
#define CCONFIGUREUTILS_H

#include <QObject>

class cConfigureUtils : public QObject
{
    Q_OBJECT
public:
    explicit cConfigureUtils(QObject *parent = nullptr);
    ~cConfigureUtils();
    static void setServerAddress(QString serverAddress);
    static void setScannerVID(QString vid);
    static void setScannerPID(QString pid);
    static void setDelayValue(int sec);
    static void setPicturesDir(QString dir);
    static void setManaualMode(QString mode);
    static void setPassword(QString password);
    static QString getServerAdddress();
    static QString getScannerVid();
    static QString getScannerPid();
    static QString getWifiMac();
    static QString getEthernetMac();
    static QString getPicturesDir();
    static QString getManualMode();
    static QString getPassword();
    static int getDelayValue();
signals:

public slots:
};

#endif // CCONFIGUREUTILS_H
