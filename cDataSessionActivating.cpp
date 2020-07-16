#include "cDataSessionActivating.h"

cDataSessionActivating::cDataSessionActivating()
{
}

cDataSessionActivating::cDataSessionActivating(const cDataSessionActivating &other)
    : m_Mnv(other.m_Mnv), m_Tenbangloi(other.m_Tenbangloi), m_Makanban(other.m_Makanban), m_Pictures(other.m_Pictures)
{

}

cDataSessionActivating::~cDataSessionActivating()
{

}

void cDataSessionActivating::setMNV(QString mnv)
{
    m_Mnv = mnv;
}

void cDataSessionActivating::setMaKanban(QString maKanban)
{
    m_Makanban = maKanban;
}

void cDataSessionActivating::setTenBangLoi(QString tenBangloi)
{
    m_Tenbangloi = tenBangloi;
}

void cDataSessionActivating::setPicturesList(QStringList pictures)
{
    m_Pictures = pictures;
}

QString cDataSessionActivating::getMNV() const
{
    return m_Mnv;
}

QString cDataSessionActivating::getMaKanban() const
{
    return m_Makanban;
}

QString cDataSessionActivating::getTenBangLoi() const
{
    return m_Tenbangloi;
}

QStringList cDataSessionActivating::getPicturesList() const
{
    return m_Pictures;
}
