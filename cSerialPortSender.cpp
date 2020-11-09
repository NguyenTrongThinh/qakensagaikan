#include "cSerialPortSender.h"
#include <QDebug>
#include "cDataUtils.h"
#include <QSerialPortInfo>
#include "cConfigureUtils.h"
#include <QString>

#define TAG "cSerialPort Sender: "

cSerialPortSender *cSerialPortSender::m_Instance = nullptr;


cSerialPortSender::cSerialPortSender(QObject *parent) : QObject(parent)
{
    m_Abort = false;
    m_IsSerialPortFound = false;
    m_IsSerialPortConnected = false;
    m_Method = GETSTATUS;
}

cSerialPortSender::~cSerialPortSender()
{

}

cSerialPortSender *cSerialPortSender::instance(QObject *parent)
{
    static QMutex mutex;
    if (m_Instance == nullptr) {
        mutex.lock();
        m_Instance = new cSerialPortSender(parent);
        mutex.unlock();
    }
    return m_Instance;
}

void cSerialPortSender::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = nullptr;
    mutex.unlock();
}

void cSerialPortSender::requestMethod(cSerialPortSender::METHOD method)
{
    QMutexLocker locker(&m_Mutex);
    m_Method = method;
}

void cSerialPortSender::abort()
{
    QMutexLocker locker(&m_Mutex);
    m_Abort = true;
}

void cSerialPortSender::setBoardData(QByteArray data)
{
    QMutexLocker locker(&m_Mutex);
    m_DataToBoard = data;
}

void cSerialPortSender::deinitSerialPort()
{
    if (m_SerialPort != nullptr)
        m_SerialPort->close();
}

bool cSerialPortSender::searchSerialPort(QSerialPortInfo *portInfo)
{
    bool retVal = false;
    // VID PID se lay tu file configure, goi ham tuw cConfigureUTils
    quint16 BOARD_PID = cConfigureUtils::getMCUPid().toUInt(nullptr,16);
    quint16 BOARD_VID = cConfigureUtils::getMCUVid().toUInt(nullptr,16);
    qDebug() << "BOARD_PID " << BOARD_PID;
    qDebug() << "BOARD_VID " << BOARD_VID;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        if(serialPortInfo.productIdentifier() == BOARD_PID && serialPortInfo.vendorIdentifier() == BOARD_VID) {
            *portInfo = serialPortInfo;
            retVal = true;
        }
    }
    return retVal;
}

void cSerialPortSender::writeData(const QByteArray &data)
{
    int retry = 3;
    bool writeDone = false;
    if(m_SerialPort != nullptr && m_SerialPort->isOpen()) {
        do {
            m_SerialPort->flush();
            m_SerialPort->clear(m_SerialPort->AllDirections);
            m_SerialPort->write(data.data(), data.length());
            writeDone = m_SerialPort->waitForBytesWritten(1000);
        } while(retry-- > 0 && !writeDone);
    }
}

QByteArray cSerialPortSender::readData(bool *ok, const int timeout, const int length)
{
    QByteArray retVal;
    QByteArray tempVal;
    *ok = false;
    int count = 10;
    int timeDelay = timeout / count;
    int tempLength = length;
    if(m_SerialPort != nullptr && m_SerialPort->isOpen()) {
        while (m_SerialPort->waitForReadyRead(timeDelay) || count-- > 0) {
            tempVal = m_SerialPort->read(tempLength);
            retVal.append(tempVal);

            if (retVal.count() == length) {
                *ok = true;
                break;
            } else {
                tempLength -= tempVal.count();
            }
        }
    }
    return retVal;
}

bool cSerialPortSender::initSerialPort(QSerialPortInfo portInfo)
{
    m_SerialPort = new QSerialPort(this);
    m_SerialPort->setPortName(portInfo.portName());
    m_SerialPort->setBaudRate(QSerialPort::Baud115200);
    m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_SerialPort->setStopBits(QSerialPort::OneStop);
    m_SerialPort->setParity(QSerialPort::NoParity);
    m_SerialPort->setDataBits(QSerialPort::Data8);
    bool retVal = m_SerialPort->open(QIODevice::ReadWrite);
    if (retVal) {
        qDebug() << "Open Serial Port " << portInfo.portName();
    } else {
        qDebug() << "Failed to open Serial Port " << portInfo.portName();
    }
    return retVal;
}

bool cSerialPortSender::sendDataToBoard(QByteArray data)
{
    bool retVal = false;
    quint8 retry = 3;
    QByteArray tempArray, response;
    bool isReadDataOK;
    if(m_SerialPort != nullptr && m_SerialPort->isOpen()) {
        QByteArray cmdACK = cDataUtils::commandACK();
        QByteArray command = cDataUtils::commandSendDataToBoard(data);
        qDebug() << "Command: " << data;
        do {
            tempArray.clear();
            response.clear();
            writeData(command);
            if (m_SerialPort->waitForReadyRead(3000)) {
                tempArray = readData(&isReadDataOK, 2000, 1);
                if (isReadDataOK) {
                    response.append(tempArray);
                    if (response.at(0) == cmdACK.at(0)) {
                        tempArray = readData(&isReadDataOK, 2000, 2);
                        if (isReadDataOK) {
                            response.append(tempArray);
                            int length = response.at(1);
                            length = length << 8 | response.at(2);
                            tempArray = readData(&isReadDataOK, 5000, length + 1);
                            if (isReadDataOK) {
                                response.append(tempArray);
                                if (cDataUtils::isCheckSumCorrect(response)) {
                                    qDebug() << TAG << "Receive ACK";
                                    int ack = response.at(3);
                                    if (ack == cDataUtils::ACK) {
                                        retVal = true;
                                        retry = 0;
                                    }
                                } else {
                                    qDebug() << "Get One Data Package: Incorrect Checksum";
                                }
                            } else {
                                qDebug() << TAG << "Read Byte Failed, expected Length: " << length;
                            }
                        } else {
                            qDebug() << TAG << "Read 2 Byte Length Failed";
                        }
                    } else {
                        qDebug() << TAG << "Start Byte Not Match: " <<  response;
                    }
                } else {
                    qDebug() << TAG << "Read Start Byte Failed";
                }
            }
        } while (retry-- > 0);
    }

    return retVal;
}

QList<cOperator> cSerialPortSender::getOperatorStatus(bool *ok)
{
    QList<cOperator> retVal;
    quint8 retry = 3;
    *ok = false;
    QByteArray tempArray, response;
    bool isReadDataOK;
    if(m_SerialPort != nullptr && m_SerialPort->isOpen()) {
        QByteArray cmdACK = cDataUtils::commandACK();
        QByteArray cmdgetOpratorStatus = cDataUtils::commandGetOperatorStatus();
        do {
            tempArray.clear();
            response.clear();
            writeData(cmdgetOpratorStatus);
            if (m_SerialPort->waitForReadyRead(3000)) {
                tempArray = readData(&isReadDataOK, 2000, 1);
                if (isReadDataOK) {
                    response.append(tempArray);
                    if (static_cast<quint8>(response.at(0)) == static_cast<quint8>(cDataUtils::CMDSTATUS::CMD)) {
                        tempArray = readData(&isReadDataOK, 2000, 2);
                        if (isReadDataOK) {
                            response.append(tempArray);
                            int length = response.at(1);
                            length = length << 8 | response.at(2);
                            tempArray = readData(&isReadDataOK, 5000, length + 1);
                            if (isReadDataOK) {
                                response.append(tempArray);
                                if (cDataUtils::isCheckSumCorrect(response)) {
                                    qDebug() << TAG << "Receive Correct Data";
                                    qDebug() << TAG << "Data " << response;
                                    retVal = cDataUtils::parseResponseFromBoard(response);
                                    *ok = true;
                                } else {
                                    qDebug() << "Get One Data Package: Incorrect Checksum";
                                }
                            } else {
                                qDebug() << TAG << "Read Byte Failed, expected Length: " << length;
                            }
                        } else {
                            qDebug() << TAG << "Read 2 Byte Length Failed";
                        }
                    } else {
                        qDebug() << TAG << "Start Byte Not Match: " <<  response;
                    }
                } else {
                    qDebug() << TAG << "Read Start Byte Failed";
                }
            }
        } while (retry-- > 0);

    }
    return retVal;
}

void cSerialPortSender::main_loop()
{
    QByteArray boardData;
    QSerialPortInfo portInfo;
    QList<cOperator> opraratorStatus;
    bool statusOk = false;
    forever {
        m_Mutex.lock();
        if (m_Abort) {
            qDebug() << TAG << "Aborting Serial Port Worker Thread";
            m_Mutex.unlock();
            emit finished();
            return;
        }
        boardData = m_DataToBoard;
        m_Mutex.unlock();
        m_IsSerialPortFound = searchSerialPort(&portInfo);
        if (m_IsSerialPortFound) {
            if (!m_IsSerialPortConnected) {
                if (initSerialPort(portInfo)) {
                    m_IsSerialPortConnected = true;
                    emit sigSerialPortConnected();
                    qDebug() << TAG << "Serial Port Connected...";
                }
            }
        } else {
            qDebug() << TAG << "Serial Port Disonnected...";
            m_IsSerialPortConnected = false;
            deinitSerialPort();
            emit sigSerialPortDisconnected();
        }

        if (m_IsSerialPortConnected && m_IsSerialPortFound) {
            switch (m_Method) {
            case SENDDATA:
                    sendDataToBoard(boardData);
                    m_Method = GETSTATUS;
                    break;
            case GETSTATUS:
                opraratorStatus = getOperatorStatus(&statusOk);
                if (statusOk && opraratorStatus.count() > 0) {
                    emit sigOperatorStatus(opraratorStatus);
                }
                break;
            }
        }
        QThread::msleep(200);
    }
}


