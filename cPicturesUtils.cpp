#include "cPicturesUtils.h"
#include <QFile>

cPicturesUtils::cPicturesUtils(QObject *parent) : QObject(parent)
{

}

cPicturesUtils::~cPicturesUtils()
{

}

void cPicturesUtils::deletePicture(QString filePath)
{
    QFile file (filePath);
    if (file.exists())
        file.remove();
}
