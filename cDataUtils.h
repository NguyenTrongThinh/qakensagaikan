#ifndef CDATAUTILS_H
#define CDATAUTILS_H

#include <QObject>
#include "cOperator.h"

class cDataUtils : public QObject
{
    Q_OBJECT
public:
    explicit cDataUtils(QObject *parent = nullptr);
    ~cDataUtils();
    enum ACKCODE {ACK = 0x06, NACK=0x15};
    enum CMDSTATUS {CMD = 0x02};
    static quint8 checkSum(QByteArray array);
    static bool isCheckSumCorrect(QByteArray array);
    static QByteArray commandACK();
    static QByteArray commandNACK();
    static QByteArray commandSendDataToBoard(QByteArray data);
    static QByteArray commandGetOperatorStatus();
    static QList<cOperator> parseResponseFromBoard(QByteArray data);

    // Khong Co loi moi gui Magic, dong moc
};

#endif // CDATAUTILS_H
