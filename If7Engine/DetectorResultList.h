#pragma once

#include "DetectorResult.h"

#include <QList>
#include <QMultiMap>
#include <QRect>

#include <vector> // std::

#include <opencv2/core.hpp>

#include "ObjdetRawArguments.h"

class DetectorResultList
{
public:
    DetectorResultList();
    DetectorResultList(const ObjdetRawArguments raw,
                       const std::vector<cv::Rect> rects,
                       const std::vector<int> counts,
                       const std::vector<cv::Rect> allrects
                            =std::vector<cv::Rect>());

public: // const
    int count() const;
    DetectorResult at(const int ix) const;
    DetectorResult best() const;
    DetectorResult::List rankedList() const;
    QList<QRect> allRectList() const;
    QList<QRect> orphanList() const;
    QList<QRect> includedRects(const DetectorResult &result,
                               const QList<QRect> &rects);

public: // non-const
    void process(const ObjdetRawArguments raw,
                 const std::vector<cv::Rect> rects,
                 const std::vector<int> counts,
                 const std::vector<cv::Rect> allrects
                    =std::vector<cv::Rect>());
    void calculate(const ObjdetRawArguments raw);
    void addRanked(const DetectorResult &dr);
    void orphanList(const QList<QRect> ol);
    void clearAll();
    void clearInputs();
    void clearResults();

private:
    std::vector<cv::Rect> mCvRects;
    std::vector<int> mCvCounts;
    std::vector<cv::Rect> mAllCvRects;
    DetectorResult::List mRankedList;
    QList<QRect> mAllRectList;
    QList<QRect> mOrphanRectList;
};

inline int DetectorResultList::count() const { return mRankedList.count(); }
inline DetectorResult DetectorResultList::at(const int ix) const { return rankedList().at(ix); }
inline DetectorResult DetectorResultList::best() const { return rankedList().first(); }
inline DetectorResult::List DetectorResultList::rankedList() const { return mRankedList; }
inline QList<QRect> DetectorResultList::allRectList() const { return mAllRectList; }
inline QList<QRect> DetectorResultList::orphanList() const { return mOrphanRectList; }
inline void DetectorResultList::orphanList(const QList<QRect> ol) { mOrphanRectList = ol; }


