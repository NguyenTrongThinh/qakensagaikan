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

    QString getMNV() const;
    QString getMaKanban() const;
    QString getTenBangLoi() const;
    QStringList getPicturesList() const;
private:
    QString m_Mnv;
    QString m_Tenbangloi;
    QString m_Makanban;
    QStringList m_Pictures;
};
Q_DECLARE_METATYPE(cDataSessionActivating)
#endif // CDATASESSIONACTIVATING_H
