#ifndef CDATASESSION_H
#define CDATASESSION_H

#include <QObject>
#include <QMetaType>

class cDataSession
{
public:
    cDataSession();
    cDataSession(const cDataSession &other);

    ~cDataSession();
    QString getmnv() const;
    QString gettime() const;
    QString getseibango()const;
    QString getblock() const;
    QString getca() const;
    QString getdeviceid() const;
    QString getLine() const;
    QString getMHCode() const;
    QString getMHNamePlate() const;
    QString getMHDatePrint() const;

    QString getMaAB1() const;
    QString getNamePlateAB1() const;
    QString getDatePrintAB1() const;
    QString getPrefixAB1() const;
    QString getMaAB2() const;
    QString getNamePlateAB2() const;
    QString getDatePrintAB2() const;
    QString getPrefixAB2() const;

    QList<QPair<QString, int>> getloi() const;
    QStringList getHinh() const;

    void setmnv(QString mnv);
    void settime(QString time);
    void setMHCode(QString mhCode);
    void setblock(QString block);
    void setca(QString ca);
    void setdeviceid(QString deviceid);
    void setMHDatePrint(QString mhDatePrint);
    void setMHNamePlate(QString mhNamePlate);
    void setloi(QList<QPair<QString, int>> loi);
    void setHinh(QStringList hinh);
    void setLine(QString line);
    void setMaAB1(QString maab1);
    void setNamePlateAB1(QString nameplateab1);
    void setDatePrintAB1(QString dateprintab1);
    void setPrefixAB1(QString prefixab1);
    void setMaAB2(QString maab2);
    void setNamePlateAB2(QString nameplateab2);
    void setDatePrintAB2(QString dateprintab2);
    void setPrefixAB2(QString prefixab2);
private:
    QString m_mnv;
    QString m_time;
    QString m_MHCode;
    QString m_deviceid;
    QString m_line;
    QString m_MHDatePrint;
    QString m_MHNamePlate;
    QList<QPair<QString, int>> m_loi;
    QString m_block;
    QString m_ca;
    QStringList m_Hinh;
    QString m_maab1;
    QString m_prefixab1;
    QString m_nameplateab1;
    QString m_dateprintab1;
    QString m_maab2;
    QString m_prefixab2;
    QString m_nameplateab2;
    QString m_dateprintab2;
};
Q_DECLARE_METATYPE(cDataSession)
#endif // CDATASESSION_H

