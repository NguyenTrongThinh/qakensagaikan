#include "cStaffIDParserUtils.h"

cStaffIDParserUtils::cStaffIDParserUtils(QObject *parent) : QObject(parent)
{

}

cStaffIDParserUtils::~cStaffIDParserUtils()
{

}

QString cStaffIDParserUtils::getCa(QString mnv)
{
    return mnv.left(1).toUpper();
}

QString cStaffIDParserUtils::getMNV(QString mnv)
{
    return mnv.mid(1, 6).toUpper();
}
