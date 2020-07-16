#ifndef CDATAMH_H
#define CDATAMH_H

#include <QObject>

class cDataMH
{
public:
    cDataMH();
    cDataMH(const cDataMH &other);

    void setMaHang(QString mahang);
    void setSoMaAB(int somaAB);
    void setMaAB1(QString maab1);
    void setMaAB2(QString maab2);
    void setThaoTacMCU(QString thaotacMCU);
    void setChamMagic(QString chammagic);
    void setDongMoc(QString dongmoc);

    QString getMaHang();
    int getSoMaAB();
    QString getMaAB1();
    QString getMaAB2();
    QString getThaoTacMCU();
    QString getChamMagic();
    QString getDongMoc();

private:
    QString m_mahang;
    int m_somaAB = 0;
    QString m_maAB1;
    QString m_maAB2;
    QString m_thaotacMCU;
    QString m_chammagic;
    QString m_dongmoc;

signals:

};
Q_DECLARE_METATYPE(cDataMH)
#endif // CDATAMH_H
