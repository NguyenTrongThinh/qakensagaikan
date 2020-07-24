#include "cAB1CodeParserUtils.h"

cAB1CodeParserUtils::cAB1CodeParserUtils(QObject *parent) : QObject(parent)
{

}

cAB1CodeParserUtils::~cAB1CodeParserUtils()
{

}

QString cAB1CodeParserUtils::getAB1(QString ab1)
{
    return ab1.left(10).toUpper();
}

QString cAB1CodeParserUtils::getAB1DatePrint(QString ab1)
{
    if(ab1.length() == 25)
    {
        return ab1.mid(15, 6).toUpper();
    }
    else
    {
        return ab1.mid(12, 6).toUpper();
    }

}

QString cAB1CodeParserUtils::getAB1NamePlate(QString ab1)
{
    if(ab1.length() == 25)
    {
        return ab1.mid(21, 4).toUpper();
    }
    else
    {
        return ab1.mid(18, 4).toUpper();
    }

}

QString cAB1CodeParserUtils::getAB1Prefix(QString ab1)
{
    if(ab1.length() == 25)
    {
        return ab1.mid(12, 3).toUpper();
    }
    else
    {
        return "";
    }

}
