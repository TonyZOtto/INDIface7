#pragma once

#include <QSharedDataPointer>

#include "FeatureInfoData.h"
class DomValuesElement;

class FeatureInfo
{
public: // const
    QPoint leftEye() const { return data->dLeftEye; }
    QPoint rightEye() const { return data->dRightEye; }
    QQRect headBox() const { return data->dHeadBox; }
    DomValuesElement domElement(QString &aElementTag) const;

public: // non-const
    void leftEye(const QPoint pt) { data->dLeftEye = pt; }
    void rightEye(const QPoint pt) { data->dRightEye = pt; }
    void headCenter(const QPoint pt) { data->dHeadCenter = pt; }
    void eyeLine(const QLine ln) { data->dEyeLine = ln; }
    void eyeDistance(const int i) { data->dEyeDistance = i; }

    void calculate(void);

public: // shared pointer
    FeatureInfo();
    FeatureInfo(const FeatureInfo &);
    FeatureInfo &operator=(const FeatureInfo &);
    ~FeatureInfo();

private:
    QSharedDataPointer<FeatureInfoData> data;
};
