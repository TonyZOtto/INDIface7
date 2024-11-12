#include "BaseObjdetEngine.h"

#include "ObjectHelper.h"

BaseObjdetEngine::BaseObjdetEngine(QObject *parent)
    : QObject{parent}
    , cmClass(Objdet::$nullClass)
{
    setObjectName("BaseObjdetEngine:Null");
}

BaseObjdetEngine::BaseObjdetEngine(const Objdet::Class objcls, QObject *parent)
    : QObject{parent}
    , cmClass(objcls)
{
    setObjectName("BaseObjdetEngine:" + className(objcls));
}

QString BaseObjdetEngine::className(const Objdet::Class odc) const
{
    const ObjectHelper cOH(this);
    return cOH.enumKey("Class", odc);
}
