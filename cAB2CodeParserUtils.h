#ifndef CAB2CODEPARSERUTILS_H
#define CAB2CODEPARSERUTILS_H

#include <QObject>

class cAB2CodeParserUtils : public QObject
{
    Q_OBJECT
public:
    explicit cAB2CodeParserUtils(QObject *parent = nullptr);
    ~cAB2CodeParserUtils();
    static QString getAB2(QString ab2);
    static QString getAB2DatePrint(QString ab2);
    static QString getAB2NamePlate(QString ab2);
    static QString getAB2Prefix(QString ab2);
signals:

};

#endif // CAB2CODEPARSERUTILS_H
