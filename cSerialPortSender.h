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
        SENDDATA,
        GETSTATUS,
    };
    static cSerialPortSender *instance(QObject *parent = nullptr);
    static void drop();

     void requestMethod(METHOD method);
     void abort();
     void setBoardData(QByteArray data);

protected:
    void writeData(const QByteArray &data);
    QByteArray readData(bool *ok, const int timeout, const int length);
private:
    bool initSerialPort(QSerialPortInfo portInfo);
    void deinitSerialPort();
    bool searchSerialPort(QSerialPortInfo *portInfo);
    bool sendDataToBoard(QByteArray data);
    QList<cOperator> getOperatorStatus(bool *ok);

private:
    static cSerialPortSender *m_Instance;
    QSerialPort *m_SerialPort = nullptr;
    bool m_IsSerialPortFound = false;
    bool m_IsSerialPortConnected = false;
    QByteArray m_DataToBoard;
    METHOD m_Method;
    bool m_Abort = false;
    QMutex m_Mutex;
signals:
     void sigOperatorStatus(const QList<cOperator> &status);
     void sigSerialPortDisconnected();
     void sigSerialPortConnected();
     void finished();
public slots:
    void main_loop();
};

#endif // CSERIALPORTGATEWAY_H
