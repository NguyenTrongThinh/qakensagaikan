#include "cDataUtils.h"
#include <QDateTime>

#define STATUSCODE_INDEX_HIGH           0x03
#define STATUSCODE_INDEX_LOW            0x04

cDataUtils::cDataUtils(QObject *parent) : QObject(parent)
{

}

cDataUtils::~cDataUtils()
{

}

quint8 cDataUtils::checkSum(QByteArray array)
{
    quint32 sum = 0;
    quint8 retVal = 0;
    for (int i = 0; i < array.length(); i++) {
        sum += static_cast<quint8>(array.at(i));
    }
    retVal = static_cast<quint8>(sum&0xFF);
    return retVal;
}

bool cDataUtils::isCheckSumCorrect(QByteArray array)
{
    bool retVal = false;
    QByteArray tempArary = array;
    if (tempArary.length() > 2) {
        quint8 checksum = tempArary.at(tempArary.length() - 1);
        tempArary.remove(tempArary.length() - 1, 1);
        if (checkSum(tempArary) == checksum) {
            retVal = true;
        }
    }
    return retVal;
}

QByteArray cDataUtils::commandACK()
{
    QByteArray retVal;
    retVal.append(static_cast<char>(0xFF));
    retVal.append(static_cast<char>(0x00));
    retVal.append(static_cast<char>(0x01));
    retVal.append(static_cast<char>(ACK));
    retVal.append(static_cast<char>(checkSum(retVal)));
    return retVal;
}

QByteArray cDataUtils::commandNACK()
{
    QByteArray retVal;
    retVal.append(static_cast<char>(0xFF));
    retVal.append(static_cast<char>(0x00));
    retVal.append(static_cast<char>(0x01));
    retVal.append(static_cast<char>(NACK));
    retVal.append(static_cast<char>(checkSum(retVal)));
    return retVal;
}

QByteArray cDataUtils::commandSendDataToBoard(QByteArray data)
{
    QByteArray retVal;
    quint16 length = data.length();
    retVal.append(static_cast<char>(0x01));
    retVal.append(static_cast<char>(length >> 8));
    retVal.append(static_cast<char>(length));
    retVal.append(data);
    retVal.append(static_cast<char>(checkSum(retVal)));
    return retVal;

}

QByteArray cDataUtils::commandGetOperatorStatus()
{
    QByteArray retVal;
    quint16 length = 0x00;
    retVal.append(static_cast<char>(0x02));
    retVal.append(static_cast<char>(length >> 8));
    retVal.append(static_cast<char>(length));
    retVal.append(static_cast<char>(checkSum(retVal)));
    return retVal;
}

QList<cOperator> cDataUtils::parseResponseFromBoard(QByteArray data)
{
    QList<cOperator> retVal;
    cOperator tmpVal;
    quint16 numOP = data.at(STATUSCODE_INDEX_HIGH);
    quint8 statusCode = data.at(STATUSCODE_INDEX_LOW);
    for (int i = 0; i < 8; i++) {
        quint8 m_operator = static_cast<quint8>(statusCode >> i) & 0x01;
        if (m_operator == 0x01) {
            tmpVal.setOperator(i);
            tmpVal.setStatusCode(m_operator);
            tmpVal.setNumOP(numOP);
            retVal.append(tmpVal);
        }
    }
    return retVal;
}
