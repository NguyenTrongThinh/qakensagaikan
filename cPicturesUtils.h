#ifndef CPICTURESUTILS_H
#define CPICTURESUTILS_H

#include <QObject>

class cPicturesUtils : public QObject
{
    Q_OBJECT
public:
    explicit cPicturesUtils(QObject *parent = 0);
    ~cPicturesUtils();
    static void deletePicture(QString filePath);
};

#endif // CPICTURESUTILS_H
