#ifndef CAB1CODEPARSERUTILS_H
#define CAB1CODEPARSERUTILS_H

#include <QObject>

class cAB1CodeParserUtils : public QObject
{
    Q_OBJECT
public:
    explicit cAB1CodeParserUtils(QObject *parent = nullptr);
    ~cAB1CodeParserUtils();
    static QString getAB1(QString ab1);
    static QString getAB1DatePrint(QString ab1);
    static QString getAB1NamePlate(QString ab1);
    static QString getAB1Prefix(QString ab1);

signals:

};

#endif // CAB1CODEPARSERUTILS_H
