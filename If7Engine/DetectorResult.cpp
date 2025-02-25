#include "DetectorResult.h"


DetectorResult::DetectorResult(QRect rect)
    : mRank(0)
    , mResultRect(rect)
    , mAverageCenter(rect.center())
    , mAverageSize(rect.size())
{
    mAllRects.append(rect);
}

DetectorResult::DetectorResult(int rank, QRect rect, QList<QRect> allRects)
    : mRank(rank)
    , mResultRect(rect)
    , mAllRects(allRects)
{

}

void DetectorResult::addToAverage(const QRect rect)
{
    mAllRects.append(rect);
    mAverageCenter.setX(mAverageCenter.x() + rect.center().x());
    mAverageCenter.setY(mAverageCenter.y() + rect.center().y());
    mAverageSize.setWidth(mAverageSize.width() + rect.width());
    mAverageSize.setHeight(mAverageSize.height() + rect.height());
    qreal n = mAllRects.size();
    mResultRect.setX(mAverageCenter.x() / n);
    mResultRect.setY(mAverageCenter.y() / n);
    mResultRect.setWidth(mAverageSize.width() / n);
    mResultRect.setHeight(mAverageSize.height() / n);
} // addToAverate()
