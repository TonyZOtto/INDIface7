#include "Objdet.h"

#include <QMetaEnum>
#include <QMetaObject>

#include <opencv2/opencv.hpp>

#include "ObjectHelper.h"

Objdet::Objdet(QObject *parent)
    : QObject{parent}
    , cmClass($nullClass)
{
    setObjectName("Objdet:Null");
}

Objdet::~Objdet()
{
    ;
}

Objdet::Objdet(const Class objcls, QObject *parent)
    : QObject{parent}
    , cmClass(objcls)
{
    setObjectName("Objdet:" + className(objcls));
}

// -------------------------- static ------------------------

VersionInfo Objdet::cvVersion()
{
    VersionInfo ver(CV_MAJOR_VERSION,
                    CV_MINOR_VERSION,
                    CV_SUBMINOR_VERSION,
                    0,
                    CV_VERSION,
                    "Copyright (c) 2000-2008, Intel Corporation. "
                    "Copyright (C) 2015-2024, OpenCV Foundation, all rights reserved.",
                    "OpenCV",
                    "Computer Vision Library");
    return ver;
}

bool Objdet::isValid(const Class objcls)
{
    return objcls > $nullClass && objcls < $maxClass;
}

Objdet::Class Objdet::objectClass(const QString name)
{
    Class result = $nullClass;
    Objdet tOD;
    const QMetaObject * pQMO = tOD.metaObject();
    const int cCount = pQMO->enumeratorCount();
    int tIndex = 0;
    while (tIndex < cCount && $nullClass == result)
    {
        const QMetaEnum cQME = pQMO->enumerator(tIndex);
        const QString cEnumName(cQME.enumName());
        if ("Class" == cEnumName)
        {
            bool tOK = false;
            int tInt = $nullClass;
            tInt = cQME.keyToValue(qPrintable(name), &tOK);
            if (tOK) result = Class(tInt);
            break;                                      /*v-1-v*/
        }
        ++tIndex;
    }                                                   /*--1--*/
    return result;
}

QString Objdet::className(const Class objcls)
{
    QString result("Null");
    Objdet tOD(objcls);
    if (isValid(objcls))
    {
        const QMetaObject * pQMO = tOD.metaObject();
        const int cCount = pQMO->enumeratorCount();
        int tIndex = 0;
        while (tIndex < cCount && result.isEmpty())
        {
            const QMetaEnum cQME = pQMO->enumerator(tIndex);
            const QString cEnumName(cQME.enumName());
            if ("Class" == cEnumName)
                result =  cQME.valueToKey(objcls);
            ++tIndex;
        }
    }
    return result;
}
/*
QString ObjdetCatalog::className(const Objdet::Class objcls) // static
{
    Objdet od(objcls);
    const ObjectHelper cOH(od);
    return cOH.enumKey("Class", objcls);
}
*/
