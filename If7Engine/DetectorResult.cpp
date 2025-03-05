#include "DetectorResult.h"


DetectorResult::DetectorResult(const QRect rect, const int count)
    : mRank(0)
    , mQuality(0)
    , mResultRect(rect)
    , mResultCount(count)
{

}

QColor DetectorResult::qualityColor(const int midQuality) const
{
    QColor result(Qt::black);
    if (quality() > midQuality)
        result = QColor(Qt::cyan).lighter(100 + ((quality() - midQuality) / 10));
    else
        result = QColor(Qt::blue).darker(100 + ((midQuality - quality()) / 10));
    return result;
}

QColor DetectorResult::qualityTextColor(const int midQuality) const
{
    QColor result(Qt::black);
    if (quality() > midQuality)
        result = QColor(Qt::darkYellow);
    else
        result = QColor(Qt::yellow);
    return result;
}

QList<QRect> DetectorResult::takeIncludedRects(const QList<QRect> ar)
{
    QList<QRect> result;
    const int cWidth = rect().width();
    const int cMinWidth = qreal(rect().width()) / 1.3;
    const int cMaxWidth = qreal(rect().width()) * 1.3;
    foreach (const QRect rc, ar)
        if (rect().contains(rc.center())
                && cWidth > cMinWidth && cWidth < cMaxWidth)
            mIncludedRects << rc;
        else
            result << rc;
    return result;
}

#if 0
void DetectorResult::addRect(QRect Rect)
{
    mResultRect = mResultRect.united(Rect);
    mAllRects.append(Rect);
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
#endif

