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
    int getsoto() const;
    int getsosoi() const;
    QList<QPair<QString, int>> getloi() const;
    QStringList getHinh() const;

    void setmnv(QString mnv);
    void settime(QString time);
    void setseibango(QString seibango);
    void setblock(QString block);
    void setca(QString ca);
    void setdeviceid(QString deviceid);
    void setsoto(int soto);
    void setsosoi(int sosoi);
    void setloi(QList<QPair<QString, int>> loi);
    void setHinh(QStringList hinh);
private:
    QString m_mnv;
    QString m_time;
    QString m_seibango;
    QString m_deviceid;
    int m_soto;
    int m_sosoi;
    QList<QPair<QString, int>> m_loi;
    QString m_block;
    QString m_ca;
    QStringList m_Hinh;

};
Q_DECLARE_METATYPE(cDataSession)
#endif // CDATASESSION_H

