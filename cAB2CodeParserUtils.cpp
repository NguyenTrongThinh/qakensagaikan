#include "cAB2CodeParserUtils.h"

cAB2CodeParserUtils::cAB2CodeParserUtils(QObject *parent) : QObject(parent)
{

}

cAB2CodeParserUtils::~cAB2CodeParserUtils()
{

}

QString cAB2CodeParserUtils::getAB2(QString ab2)
{
    return ab2.left(10).toUpper();
}

QString cAB2CodeParserUtils::getAB2DatePrint(QString ab2)
{
    if(ab2.length() == 25)
    {
        return ab2.mid(15, 6).toUpper();
    }
    else
    {
        return ab2.mid(12, 6).toUpper();
    }
}

QString cAB2CodeParserUtils::getAB2NamePlate(QString ab2)
{
    if(ab2.length() == 25)
    {
        return ab2.mid(21, 4).toUpper();
    }
    else
    {
        return ab2.mid(18, 4).toUpper();
    }
}

QString cAB2CodeParserUtils::getAB2Prefix(QString ab2)
{
    if(ab2.length() == 25)
    {
        return ab2.mid(12, 3).toUpper();
    }
    else
    {
        return "";
    }
}
