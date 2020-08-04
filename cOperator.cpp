#include "cOperator.h"

cOperator::cOperator()
{

}

cOperator::cOperator(const cOperator &other)
    :m_Operator(other.m_Operator), m_statusCode(other.m_statusCode), m_NumOP(other.m_NumOP)
{

}

cOperator::~cOperator()
{

}

void cOperator::setOperator(quint8 operatorCode)
{
    m_Operator = operatorCode;
}

void cOperator::setStatusCode(quint8 statuscode)
{
    m_statusCode = statuscode;
}

void cOperator::setNumOP(quint8 numOP)
{
    m_NumOP = numOP;
}

quint8 cOperator::getOperator() const
{
    return m_Operator;
}

quint8 cOperator::getStatusCode() const
{
    return m_statusCode;
}

quint8 cOperator::getNumOP() const
{
    return m_NumOP;
}
