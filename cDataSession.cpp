#include "cDataSession.h"


cDataSession::cDataSession()
{

}
cDataSession::cDataSession(const cDataSession &other)
    :m_mnv(other.m_mnv), m_time(other.m_time), m_seibango(other.m_seibango), m_deviceid(other.m_deviceid), m_soto(other.m_soto), m_sosoi(other.m_sosoi),  m_loi(other.m_loi), m_block(other.m_block), m_ca(other.m_ca), m_Hinh(other.m_Hinh)
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

QString cDataSession::getseibango() const
{
    return m_seibango;
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

int cDataSession::getsoto() const
{
    return m_soto;
}

int cDataSession::getsosoi() const
{
    return m_sosoi;
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

void cDataSession::setseibango(QString seibango)
{
    m_seibango = seibango;
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

void cDataSession::setsoto(int soto)
{
    m_soto = soto;
}

void cDataSession::setsosoi(int sosoi)
{
    m_sosoi = sosoi;
}

void cDataSession::setloi(QList<QPair<QString, int> > loi)
{
    m_loi = loi;
}

void cDataSession::setHinh(QStringList hinh)
{
    m_Hinh = hinh;
}
