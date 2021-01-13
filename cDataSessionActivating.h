#ifndef CDATASESSIONACTIVATING_H
#define CDATASESSIONACTIVATING_H

#include <QObject>
#include <QMetaType>

class cDataSessionActivating
{
public:
    cDataSessionActivating();
    cDataSessionActivating(const cDataSessionActivating &other);
    ~cDataSessionActivating();

    void setMNV(QString mnv);
    void setMaKanban(QString maKanban);
    void setTenBangLoi(QString tenBangloi);
    void setPicturesList(QStringList pictures);
    void setMaAB1(QString maab1);
    void setMaAB2(QString maab2);
    void setMCUAction(QString mcuaction);
    void setMagic(QString magic);
    void setDongMoc(QString dongmoc);

    QString getMNV() const;
    QString getMaKanban() const;
    QString getTenBangLoi() const;
    QStringList getPicturesList() const;
    QString getMaAB1() const;
    QString getMaAB2() const;
    QString getMCUAction() const;
    QString getMagic() const;
    QString getDongMoc() const;
private:
    QString m_Mnv;
    QString m_Tenbangloi;
    QString m_Makanban;
    QString m_maab1;
    QString m_maab2;
    QString m_thaotacmcu;
    QString m_magic;
    QString m_dongmoc;
    QStringList m_Pictures;
};
Q_DECLARE_METATYPE(cDataSessionActivating)
#endif // CDATASESSIONACTIVATING_H
