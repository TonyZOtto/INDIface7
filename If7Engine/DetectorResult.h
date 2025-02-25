#pragma once

#include <QList>
#include <QPointF>
#include <QRect>
#include <QSizeF>

class DetectorResult
{
public:

public:
    DetectorResult(QRect rect);
    DetectorResult(int rank=0, QRect rect=QRect(),
                   QList<QRect> allRects=QList<QRect>());

public:
    int rank(void) { return mRank;; }
    int score(void) const { return mQuality; }
    QRect rectangle(void) const { return mResultRect; }
    QList<QRect> allRectangles(void) { return mAllRects; }

private:
    void addRect(QRect Rect) { mResultRect = mResultRect.united(Rect); mAllRects.append(Rect); }
    void addToAverage(const QRect rect);

private:
    int mRank;
    int mQuality;
    QRect mResultRect;
    QPointF mAverageCenter;
    QSizeF mAverageSize;
    QList<QRect> mAllRects;
};
