#include "Objdet.h"

#include <QMetaEnum>
#include <QMetaObject>

#include <opencv2/opencv.hpp>

Objdet::Objdet(QObject *parent)
    : QObject{parent}
{
    setObjectName("Objdet");
}


VersionInfo Objdet::cvVersion() const
{
    VersionInfo ver(CV_MAJOR_VERSION,
                    CV_MINOR_VERSION,
                    CV_SUBMINOR_VERSION,
                    0,
                    CV_VERSION,
                    "Copyright (c) 2000-2008, Intel Corporation. "
                    "(c) 2008-2010, Willow Garage Inc.",
                    "Intel",
                    "Computer Vision Library");
    return ver;
}

QString Objdet::className(const Class objcls)
{
    QString result;
    const QMetaObject * pQMO = metaObject();
    const int cCount = pQMO->enumeratorCount();
    int tIndex = 0;
    while (tIndex < cCount && result.isEmpty())
    {
        const QMetaEnum cQME = pQMO->enumerator(tIndex);
        const QString cEnumName(cQME.enumName());
        if ("Class" == cEnumName)
            result =  cQME.valueToKey(objcls);
    }
    return result;
}
