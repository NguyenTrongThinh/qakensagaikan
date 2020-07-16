#ifndef CSTAFFIDPARSERUTILS_H
#define CSTAFFIDPARSERUTILS_H

#include <QObject>

class cStaffIDParserUtils : public QObject
{
    Q_OBJECT
public:
    explicit cStaffIDParserUtils(QObject *parent = nullptr);
    ~cStaffIDParserUtils();
    static QString getCa(QString mnv);
    static QString getMNV(QString mnv);

signals:

public slots:
};

#endif // CSTAFFIDPARSERUTILS_H
