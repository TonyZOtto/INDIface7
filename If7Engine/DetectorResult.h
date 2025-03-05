#pragma once

#include <QColor>
#include <QList>
#include <QPointF>
#include <QRect>
#include <QSizeF>

class DetectorResult
{
public: // types
    typedef QList<DetectorResult> List;

public: // ctors
    DetectorResult(const QRect rect, const int count=0);

public: // const
    int rank(void) const { return mRank; }
    int quality(void) const { return mQuality; }
    QRect rect(void) const { return mResultRect; }
    int count(void) const { return mResultCount; }
    QList<QRect> includedRects(void) const { return mIncludedRects; }
    QColor qualityColor(const int midQuality) const;
    QColor qualityTextColor(const int midQuality) const;

public: // non-const
    void rank(const int r) { mRank = r; }
    void quality(const int q) { mQuality = q; }
    void count(const int k) { mResultCount = k; }
    QList<QRect> takeIncludedRects(const QList<QRect> ar);

private:
    int mRank;
    int mQuality;
    QRect mResultRect;
    int mResultCount;
    QList<QRect> mIncludedRects;
};
