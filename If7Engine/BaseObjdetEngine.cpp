#include "BaseObjdetEngine.h"

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
