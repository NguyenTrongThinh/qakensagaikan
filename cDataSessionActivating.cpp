#include "cDataSessionActivating.h"

cDataSessionActivating::cDataSessionActivating()
{
}

cDataSessionActivating::cDataSessionActivating(const cDataSessionActivating &other)
    : m_Mnv(other.m_Mnv), m_Tenbangloi(other.m_Tenbangloi), m_Makanban(other.m_Makanban), m_maab1(other.m_maab1), m_maab2(other.m_maab2) , m_thaotacmcu(other.m_thaotacmcu), m_magic(other.m_magic), m_dongmoc(other.m_dongmoc), m_Pictures(other.m_Pictures)
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

void cDataSessionActivating::setMaAB1(QString maab1)
{
    m_maab1 = maab1;
}

void cDataSessionActivating::setMaAB2(QString maab2)
{
    m_maab2 = maab2;
}

void cDataSessionActivating::setMCUAction(QString mcuaction)
{
    m_thaotacmcu = mcuaction;
}

void cDataSessionActivating::setMagic(QString magic)
{
    m_magic = magic;
}

void cDataSessionActivating::setDongMoc(QString dongmoc)
{
    m_dongmoc = dongmoc;
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

QString cDataSessionActivating::getMaAB1() const
{
    return m_maab1;
}

QString cDataSessionActivating::getMaAB2() const
{
    return m_maab2;
}

QString cDataSessionActivating::getMCUAction() const
{
    return m_thaotacmcu;
}

QString cDataSessionActivating::getMagic() const
{
    return m_magic;
}

QString cDataSessionActivating::getDongMoc() const
{
    return m_dongmoc;
}
