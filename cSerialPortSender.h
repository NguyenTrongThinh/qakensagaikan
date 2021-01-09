#ifndef CSERIALPORTGATEWAY_H
#define CSERIALPORTGATEWAY_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include "cOperator.h"

class cSerialPortSender : public QObject
{
    Q_OBJECT
public:
    explicit cSerialPortSender(QObject *parent = nullptr);
    ~cSerialPortSender();
    enum METHOD {
        DONOTHING,
        SENDDATA,
        GETSTATUS,
        SENDMAGICDATA,
        GETMAGICSTATUS,
        SENDCARPENTRYDATA,
        GETCARPENTRYSTATUS,
    };
    static cSerialPortSender *instance(QObject *parent = nullptr);
    static void drop();

     void requestMethod(METHOD method);
     void abort();
     void setBoardData(QByteArray data);
     void setMagicData(QByteArray magicData);
     void setCarpentryData(QByteArray carpentryData);

protected:
    void writeData(const QByteArray &data);
    QByteArray readData(bool *ok, const int timeout, const int length);
private:
    bool initSerialPort(QSerialPortInfo portInfo);
    void deinitSerialPort();
    bool searchSerialPort(QSerialPortInfo *portInfo);
    bool sendDataToBoard(QByteArray data);
    bool sendMagicDataToBoard(QByteArray magicData);
    bool sendCarpentryDataToBoard(QByteArray carpentryData);
    QList<cOperator> getOperatorStatus(bool *ok);
    bool getMagicStatus(bool *ok);
    bool getCarpentryStatus(bool *ok);

private:
    static cSerialPortSender *m_Instance;
    QSerialPort *m_SerialPort = nullptr;
    bool m_IsSerialPortFound = false;
    bool m_IsSerialPortConnected = false;
    QByteArray m_DataToBoard;
    QByteArray m_MagicData;
    QByteArray m_CarpentryData;
    METHOD m_Method;
    bool m_Abort = false;
    QMutex m_Mutex;
signals:
     void sigOperatorStatus(const QList<cOperator> &status);
     void sigMagicStatus(const bool &status);
     void sigCarpentryStatus(const bool &status);
     void sigSerialPortDisconnected();
     void sigSerialPortConnected();
     void finished();
public slots:
    void main_loop();
};

#endif // CSERIALPORTGATEWAY_H
