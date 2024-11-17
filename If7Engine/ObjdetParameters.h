#pragma once

#include <QObject>

class ObjdetParameters
{
public:
    ObjdetParameters();

    // ------------------ properties ---------------------
private:
    bool m_Enable;

    qreal m_Factor;
    int m_Flags;
    QSize m_MinSize;
    QSize m_MaxSize;

    int m_MaxDensity;
    int m_MaxPixels;
    int m_MaxDimension;
    int m_MaxDetectors;
    int m_MaxResults;
    bool m_ForceFind;
    int m_InputOverCrop;
    int m_MinAcross;
    int m_MaxAcross;
    int m_MinQuality;


    QString m_DetectorFile;

    bool m_MarkAll;
    QString m_MarkColors;
    QString m_MarkScaleColor;
    QString m_MarkExtraColor;


};
