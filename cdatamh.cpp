#include "cdatamh.h"

cDataMH::cDataMH()
{

}

cDataMH::cDataMH(const cDataMH &other):m_mahang(other.m_mahang), m_somaAB(other.m_somaAB), m_maAB1(other.m_maAB1), m_maAB2(other.m_maAB2), m_thaotacMCU(other.m_thaotacMCU), m_chammagic(other.m_chammagic), m_dongmoc(other.m_dongmoc)
{

}

void cDataMH::setMaHang(QString mahang)
{
    m_mahang = mahang;
}

void cDataMH::setSoMaAB(int somaAB)
{
    m_somaAB = somaAB;
}

void cDataMH::setMaAB1(QString maab1)
{
    m_maAB1 = maab1;
}

void cDataMH::setMaAB2(QString maab2)
{
    m_maAB2 = maab2;
}

void cDataMH::setThaoTacMCU(QString thaotacMCU)
{
    m_thaotacMCU = thaotacMCU;
}

void cDataMH::setChamMagic(QString chammagic)
{
    m_chammagic = chammagic;
}

void cDataMH::setDongMoc(QString dongmoc)
{
    m_dongmoc = dongmoc;
}

QString cDataMH::getMaHang()
{
    return  m_mahang;
}

int cDataMH::getSoMaAB()
{
    return m_somaAB;
}

QString cDataMH::getMaAB1()
{
    return m_maAB1;
}

QString cDataMH::getMaAB2()
{
    return m_maAB2;
}

QString cDataMH::getThaoTacMCU()
{
    return m_thaotacMCU;
}

QString cDataMH::getChamMagic()
{
    return m_chammagic;
}

QString cDataMH::getDongMoc()
{
    return m_dongmoc;
}
