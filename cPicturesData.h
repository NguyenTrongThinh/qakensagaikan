#ifndef CPICTURESDATA_H
#define CPICTURESDATA_H

#include <QObject>
#include <QMetaType>
#include <QDateTime>

class cPicturesData
{
public:
    cPicturesData();
    cPicturesData(const cPicturesData &other);
    ~cPicturesData();

    QString getLocationOnDisk() const;
    QDateTime getSaveDate() const;

    void setLocationOnDisk(QString filePath);
    void setSaveDate(QDateTime saveDate);
private:
    QString m_LocationOnDisk;
    QDateTime m_SaveDate;
};
Q_DECLARE_METATYPE(cPicturesData)
#endif // CPICTURESDATA_H
