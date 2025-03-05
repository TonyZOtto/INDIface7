#include "DetectorResultList.h"

#include <QtDebug>

#include "ObjdetRawArguments.h"

DetectorResultList::DetectorResultList() {;}

DetectorResultList::DetectorResultList(const ObjdetRawArguments raw,
                                       const std::vector<cv::Rect> rects,
                                       const std::vector<int> counts,
                                       const std::vector<cv::Rect> allrects)
    : mCvRects(rects)
    , mCvCounts(counts)
    , mAllCvRects(allrects)
{
    calculate(raw);
}

void DetectorResultList::process(const ObjdetRawArguments raw,
                                 const std::vector<cv::Rect> rects,
                                 const std::vector<int> counts,
                                 const std::vector<cv::Rect> allrects)
{
    mCvRects = rects, mCvCounts = counts, mAllCvRects = allrects;
    calculate(raw);
}

void DetectorResultList::calculate(const ObjdetRawArguments raw)
{
    clearResults();
    if (0 == mCvRects.size()
            || 0 == mCvCounts.size()
            || mCvRects.size() != mCvCounts.size())
        qCritical() << "Rects/Counts mismatch"
                    << mCvRects.size() << mCvCounts.size();
    for (unsigned ix = 0; ix < mAllCvRects.size(); ++ix)
        mAllRectList << QRect(QPoint(mAllCvRects.at(ix).tl().x,
                                        mAllCvRects.at(ix).tl().y),
                                 QSize(mAllCvRects.at(ix).width,
                                       mAllCvRects.at(ix).height));

    const unsigned cItemCount = qMin(mCvRects.size(), mCvCounts.size());
    for (unsigned ix = 0; ix < cItemCount; ++ix)
    {
        const QRect cObjdetRect(QPoint(mAllCvRects.at(ix).tl().x,
                                       mAllCvRects.at(ix).tl().y),
                                QSize(mAllCvRects.at(ix).width,
                                      mAllCvRects.at(ix).height));
        const int cObjdetCount = mCvCounts.at(ix);
    }

    // MUSTDO MUCHO

}

void DetectorResultList::addRanked(const DetectorResult &dr)
{
    mRankedList.append(dr);
}

void DetectorResultList::clearAll()
{
    clearInputs(), clearResults();
}

void DetectorResultList::clearInputs()
{
    mCvRects.clear(), mCvCounts.clear(), mAllCvRects.clear();
}

void DetectorResultList::clearResults()
{
    mRankedList.clear(), mOrphanRectList.clear();
}
