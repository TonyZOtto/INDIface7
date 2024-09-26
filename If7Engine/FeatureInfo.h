#pragma once

#include <QSharedDataPointer>

#include "FeatureInfoData.h"
class DomValuesElement;

class FeatureInfo
{
public: // const
    QString imageSource() const { return data->dImageSource; }
    QSize imageSize() const { return data->dImageSize; }
    QString headMethod() const { return data->dHeadMethod; }
    QPoint leftEye() const { return data->dLeftEye; }
    QPoint rightEye() const { return data->dRightEye; }
    QPoint headCenter() const { return data->dHeadCenter; }
    QLine eyeLine() const { return data->dEyeLine; }
    int eyeDistance() const { return data->dEyeDistance; }
    QQRect headBox() const { return data->dHeadBox; }
    QString templateMethod() const { return data->dTemplateMethod; }
    int consistency() const { return data->dConsistency; }
    DomValuesElement domElement(QString &aElementTag) const;

public: // non-const
    void clear();
    void merge2(const FeatureInfo &other);
    void imageSource(const QString &s) { data->dImageSource = s; }
    void imageSize(const QSize &s) { data->dImageSize = s; }
    void leftEye(const QPoint pt) { data->dLeftEye = pt; }
    void rightEye(const QPoint pt) { data->dRightEye = pt; }
    void headCenter(const QPoint pt) { data->dHeadCenter = pt; }
    void eyeLine(const QLine ln) { data->dEyeLine = ln; }
    void eyeDistance(const int i) { data->dEyeDistance = i; }
    void headBox(const QQRect rc) { data->dHeadBox = rc; }
    void templateMethod(const QString &s) { data->dTemplateMethod = s; }
    void consistency(const int i) { data->dConsistency = i; }
    void calculate(void);

public: // shared pointer
    FeatureInfo();
    FeatureInfo(const FeatureInfo &);
    FeatureInfo &operator=(const FeatureInfo &);
    ~FeatureInfo();

private:
    QSharedDataPointer<FeatureInfoData> data;
};
