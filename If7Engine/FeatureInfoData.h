#pragma once

#include <QLine>
#include <QPoint>
#include <QSize>

#include "QQRect.h"

class FeatureInfoData : public QSharedData
{
public:
    QString     dImageSource;
    qint64      dTimeStampEMS;
    QSize       dImageSize;
    QString     dUsage; //??
    QString     dHeadMethod;
    int         dQuality;
    QQRect      dHeadBox;
    QPoint      dHeadCenter;
    int         dEyeMethod;
    QPoint      dLeftEye;
    QPoint      dRightEye;
    QLine       dEyeLine;
    int         dEyeDistance;
    QString     dTemplateMethod;
    int         dConsistency;
};
