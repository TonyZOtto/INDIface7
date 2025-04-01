#include "DetectorResult.h"


DetectorResult::DetectorResult(const QRect rect, const int count)
    : mRank(0)
    , mQuality(0)
    , mResultRect(rect)
    , mResultCount(count)
{

}

QList<QRect> DetectorResult::includedQRects() const
{
    QList<QRect> result;
    foreach (const SCRect cSCR, includedRects())
        result << cSCR.toQRect();
    return result;
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

QList<SCRect> DetectorResult::takeIncludedRects(const QList<SCRect> ar)
{
    QList<SCRect> result;
    const int cMinWidth = qreal(rect().width()) / 1.3;
    const int cMaxWidth = qreal(rect().width()) * 1.3;
    foreach (const SCRect rc, ar)
        if (rect().contains(rc.center())
                && rc.width() > cMinWidth
                && rc.width() < cMaxWidth)
            mIncludedRects << rc;
        else
            result << rc;
    return result;
}

