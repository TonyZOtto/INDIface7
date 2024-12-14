#include "FrameObjdet.h"

#include "FrontalMarker.h"
#include "FrontalObjdet.h"

FrameObjdet::FrameObjdet(QObject *parent)
    : QObject{parent}
    , cmpObjdet(new FrontalObjdet(this))
    , cmpMarker(new FrontalMarker(this))
{
    setObjectName("FrameObjdet");
    Q_ASSERT(cmpObjdet);
    Q_ASSERT(cmpMarker);
}
