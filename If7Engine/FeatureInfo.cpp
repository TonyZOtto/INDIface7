#include "FeatureInfo.h"

#include "DomValuesElement.h"

DomValuesElement FeatureInfo::domElement(QString &aElementTag) const
{
    DomValuesElement result(aElementTag);
    result.append(data->dImageSource, "ImageSource");
    return result;
}

void FeatureInfo::clear()
{
    leftEye(QPoint());
    rightEye(QPoint());
    headCenter(QPoint());
    eyeLine(QLine());
    eyeDistance(0);
}

void FeatureInfo::merge2(const FeatureInfo &other)
{
    if (leftEye().isNull())     leftEye(other.leftEye());
    if (rightEye().isNull())    rightEye(other.rightEye());
    if (headCenter().isNull())  headCenter(other.headCenter());
    if (eyeLine().isNull())     eyeLine(other.eyeLine());
    if (eyeDistance().isNull()) eyeDistance(other.eyeDistance());
}

void FeatureInfo::calculate()
{
    const QPoint cLEye = leftEye();
    const QPoint cREye = rightEye();
    const QQRect cHeadBox = headBox();

    if ( ! cLEye.isNull() && ! cREye.isNull())
    {
        const QLine cEyeLine(cLEye, cREye);
        const qreal cDistSquared = cEyeLine.dx() * cEyeLine.dx()
                            + cEyeLine.dy() * cEyeLine.dy();
        eyeLine(cEyeLine);
        eyeDistance(int(sqrt(cDistSquared)));
        const QPoint cCenterPt = (cLEye + cREye) / 2;
        headCenter(cCenterPt);
    }
    else if ( ! cHeadBox.isNull())
    {
        headCenter(cHeadBox.center());
    }
}

FeatureInfo::FeatureInfo()
    : data(new FeatureInfoData)
{}

FeatureInfo::FeatureInfo(const FeatureInfo &rhs)
    : data{rhs.data}
{}

FeatureInfo &FeatureInfo::operator=(const FeatureInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

FeatureInfo::~FeatureInfo() {}
