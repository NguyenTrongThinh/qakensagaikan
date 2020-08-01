#include "cDataSession.h"
#include <QDebug>

cDataSession::cDataSession()
{

}
cDataSession::cDataSession(const cDataSession &other)
    :m_mnv(other.m_mnv), m_time(other.m_time), m_MHCode(other.m_MHCode), m_deviceid(other.m_deviceid), m_line(other.m_line), m_MHDatePrint(other.m_MHDatePrint), m_MHNamePlate(other.m_MHNamePlate),  m_loi(other.m_loi), m_block(other.m_block), m_ca(other.m_ca), m_Hinh(other.m_Hinh), m_maab1(other.m_maab1), m_prefixab1(other.m_prefixab1), m_nameplateab1(other.m_nameplateab1), m_dateprintab1(other.m_dateprintab1), m_maab2(other.m_maab2), m_prefixab2(other.m_prefixab2), m_nameplateab2(other.m_nameplateab2), m_dateprintab2(other.m_dateprintab2)
{

}

cDataSession::~cDataSession()
{

}


QString cDataSession::getmnv() const
{
    return m_mnv;
}

QString cDataSession::gettime() const
{
    return m_time;
}

QString cDataSession::getblock() const
{
    return m_block;
}

QString cDataSession::getca() const
{
    return m_ca;
}

QString cDataSession::getdeviceid() const
{
    return m_deviceid;
}

QString cDataSession::getLine() const
{
    return m_line;
}

QString cDataSession::getMHCode() const
{
    return m_MHCode;
}

QString cDataSession::getMHNamePlate() const
{
    return m_MHNamePlate;
}

QString cDataSession::getMHDatePrint() const
{
    return m_MHDatePrint;
}

QString cDataSession::getMaAB1() const
{
    return m_maab1;
}

QString cDataSession::getNamePlateAB1() const
{
    return m_nameplateab1;
}

QString cDataSession::getDatePrintAB1() const
{
    return m_dateprintab1;
}

QString cDataSession::getPrefixAB1() const
{
    return m_prefixab1;
}

QString cDataSession::getMaAB2() const
{
    return m_maab2;
}

QString cDataSession::getNamePlateAB2() const
{
    return m_nameplateab2;
}

QString cDataSession::getDatePrintAB2() const
{
    return  m_dateprintab2;
}

QString cDataSession::getPrefixAB2() const
{
    return m_prefixab2;
}

QList<QPair<QString, int> > cDataSession::getloi() const
{
    return m_loi;
}

QStringList cDataSession::getHinh() const
{
    return m_Hinh;
}

void cDataSession::setmnv(QString mnv)
{
    m_mnv = mnv;
}

void cDataSession::settime(QString time)
{
    m_time = time;
}

void cDataSession::setMHCode(QString mhCode)
{
    m_MHCode = mhCode;
}

void cDataSession::setblock(QString block)
{
    m_block = block;
}

void cDataSession::setca(QString ca)
{
    m_ca = ca;
}

void cDataSession::setdeviceid(QString deviceid)
{
    m_deviceid = deviceid;
}

void cDataSession::setMHDatePrint(QString mhDatePrint)
{
    m_MHDatePrint = mhDatePrint;
}

void cDataSession::setMHNamePlate(QString mhNamePlate)
{
    m_MHNamePlate = mhNamePlate;
}

void cDataSession::setloi(QList<QPair<QString, int> > loi)
{
    m_loi = loi;
}

void cDataSession::setHinh(QStringList hinh)
{
    m_Hinh = hinh;
}

void cDataSession::setLine(QString line)
{
    m_line = line;
}

void cDataSession::setMaAB1(QString maab1)
{
    m_maab1 = maab1;
}

void cDataSession::setNamePlateAB1(QString nameplateab1)
{
    m_nameplateab1 = nameplateab1;
}

void cDataSession::setDatePrintAB1(QString dateprintab1)
{
    m_dateprintab1 = dateprintab1;
}

void cDataSession::setPrefixAB1(QString prefixab1)
{
    m_prefixab1 = prefixab1;
}

void cDataSession::setMaAB2(QString maab2)
{
    m_maab2 = maab2;
}

void cDataSession::setNamePlateAB2(QString nameplateab2)
{
    m_nameplateab2 = nameplateab2;
}

void cDataSession::setDatePrintAB2(QString dateprintab2)
{
    m_dateprintab2 = dateprintab2;
}

void cDataSession::setPrefixAB2(QString prefixab2)
{
    m_prefixab2 = prefixab2;
}


