#include "ObjdetParametersRaw.h"

ObjdetParametersRaw::ObjdetParametersRaw(QObject *parent)
    : QObject{parent}
{
    reset_Factor();
    reset_Neighbors();
    reset_Flags();
    reset_MinSize();
    reset_MaxSize();
}

cv::Size ObjdetParametersRaw::cvMinSize() const
{
    return cv::Size(mMinSize.width(), mMinSize.height());
}

cv::Size ObjdetParametersRaw::cvMaxSize() const
{
    return cv::Size(mMaxSize.width(), mMaxSize.height());
}
