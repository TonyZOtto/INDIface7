#pragma once

#include <QColor>
#include <QList>
#include <QPointF>
#include <QRect>
#include <QSizeF>

#include "SCRect.h"

class DetectorResult
{
public: // types
    typedef QList<DetectorResult> List;

public: // ctors
    DetectorResult(const QRect rect=QRect(), const int count=0);

public: // const
    int rank(void) const { return mRank; }
    int quality(void) const { return mQuality; }
    SCRect rect(void) const { return mResultRect; }
    int count(void) const { return mResultCount; }
    QList<SCRect> includedRects(void) const { return mIncludedRects; }
    QList<QRect> includedQRects(void) const;
    QColor qualityColor(const int midQuality) const;
    QColor qualityTextColor(const int midQuality) const;

public: // non-const
    void rank(const int r) { mRank = r; }
    void quality(const int q) { mQuality = q; }
    void count(const int k) { mResultCount = k; }
    QList<SCRect> takeIncludedRects(const QList<SCRect> ar);

private:
    int mRank;
    int mQuality;
    SCRect mResultRect;
    int mResultCount;
    QList<SCRect> mIncludedRects;
};
