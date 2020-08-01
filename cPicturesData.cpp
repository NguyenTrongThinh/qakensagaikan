#include "cPicturesData.h"
// :
cPicturesData::cPicturesData()
{

}

cPicturesData::cPicturesData(const cPicturesData &other)
    : m_LocationOnDisk(other.m_LocationOnDisk), m_SaveDate(other.m_SaveDate)
{

}

cPicturesData::~cPicturesData()
{

}

QString cPicturesData::getLocationOnDisk() const
{
    return m_LocationOnDisk;
}

QDateTime cPicturesData::getSaveDate() const
{
    return m_SaveDate;
}

void cPicturesData::setLocationOnDisk(QString filePath)
{
    m_LocationOnDisk = filePath;
}

void cPicturesData::setSaveDate(QDateTime saveDate)
{
    m_SaveDate = saveDate;
}
