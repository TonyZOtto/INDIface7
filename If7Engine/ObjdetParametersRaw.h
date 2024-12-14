#pragma once

#include <QObject>

#include <QSize>

#include <opencv2/objdetect.hpp>

class ObjdetParametersRaw : public QObject
{
    Q_OBJECT
public: // types
    enum Flag
    {
        $null = 0,
        Canny = cv::CASCADE_DO_CANNY_PRUNING,
        Scale = cv::CASCADE_SCALE_IMAGE,
        Biggest = cv::CASCADE_FIND_BIGGEST_OBJECT,
        Rough = cv::CASCADE_DO_ROUGH_SEARCH,
        ForceRaw = 0x8000,
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    Q_FLAG(Flags)

public: // ctors
    explicit ObjdetParametersRaw(QObject *parent = nullptr);

signals:


    // ------------------ properties ---------------------
private:
    qreal m_Factor;
    int m_Neighbors;
    int m_Flags;
    QSize m_MinSize;
    QSize m_MaxSize;

};


