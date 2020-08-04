#ifndef COPERATOR_H
#define COPERATOR_H

#include <QObject>
#include <QMetaType>

class cOperator
{
public:
    cOperator();
    cOperator(const cOperator &other);
    ~cOperator();
    enum STATUSCODE {SUCCESS = 0x00, ERROR=0x01};
    void setOperator(quint8 operatorCode);
    void setStatusCode(quint8 statuscode);
    void setNumOP(quint8 numOP);

    quint8 getOperator() const;
    quint8 getStatusCode() const;
    quint8 getNumOP() const;
private:
    quint8 m_Operator = 0;
    quint8 m_statusCode = ERROR;
    quint8 m_NumOP = 0;
};
Q_DECLARE_METATYPE(cOperator)
#endif // COPERATOR_H
