#include "Objdet.h"

#include <QMetaEnum>
#include <QMetaObject>

#include <opencv2/opencv.hpp>

#include "ObjectHelper.h"
#include "ObjdetRawArguments.h"

Objdet::Objdet(QObject *parent)
    : QObject{parent}
    , cmClass($nullClass)
{
    setObjectName("Objdet:Null");
}

Objdet::~Objdet()
{
    if (mpCascade) delete mpCascade;
}

Objdet::Objdet(const Class objcls, QObject *parent)
    : QObject{parent}
    , cmClass(objcls)
    , mpCascade(new cv::CascadeClassifier())
{
    setObjectName("Objdet:" + QString::number(objcls));
}

void Objdet::loadDetectorXml(const QString &fileName)
{
    qDebug() << Q_FUNC_INFO << fileName;
    unloadDetector();
    QFileInfo tFI(fileName);
    if ( ! tFI.isReadable())
    {
        QString tErrMsg("File not readable:" + tFI.absoluteFilePath());
        emit error(tErrMsg);
        qCritical() << tErrMsg;
    }
#if 0
    const std::__cxx11::basic_string<char,
                                     std::char_traits<char>,
                                     std::allocator<char> >
        cStdCascadeXmlName = tFI.absoluteFilePath().toStdString();
#else
    std::basic_string<char,
                           std::char_traits<char>,
                           std::allocator<char> > const&
        cStdCascadeXmlName = tFI.absoluteFilePath().toStdString();
#endif

    Q_ASSERT(mpCascade);
    if ( ! mpCascade->load(cStdCascadeXmlName))
    {
        QString tErrMsg("File failed to load:" + tFI.absoluteFilePath());
        emit error(tErrMsg);
        qCritical() << tErrMsg;
        unloadDetector();
    }
    mCascadeFileInfo = tFI;
    Q_ASSERT( ! mpCascade->empty());
}

void Objdet::unloadDetector()
{
    Q_ASSERT(mpCascade);
    if (isDetectorLoaded())
        mpCascade->load("");
    mCascadeFileInfo = QFileInfo();
    Q_ASSERT(mpCascade->empty());
}

QImage Objdet::inputImage() const
{
    return mInputImage;
}

void Objdet::set(const ObjdetRawArguments raw)
{
    qDebug() << Q_FUNC_INFO << raw.factor() << raw.neighbors()
             << raw.flags() << raw.minSize() << raw.minSize()
             << raw.inputSize();
    mRawParms = raw;
}

void Objdet::inputImage(const QImage &img)
{
    qDebug() << Q_FUNC_INFO << img;
    if (img.isGrayscale())
        Q_ASSERT(QImage::Format_Grayscale8 == img.format());
    else
        Q_ASSERT(QImage::Format_ARGB32 == img.format());
    mInputImage = img;
}

void Objdet::clear()
{
    mAllRects.clear(), mOrphanRects.clear(),
        mResultList.clearResults();
    mInputImage = QImage(), mGreyImage = QImage();
    mGreyMat.deallocate();
    mGreyMat = cv::Mat();
}





// -------------------------- static ------------------------

VersionInfo Objdet::cvVersion()
{
    VersionInfo ver(CV_MAJOR_VERSION,
                    CV_MINOR_VERSION,
                    CV_SUBMINOR_VERSION,
                    0,
                    CV_VERSION,
                    "Copyright (c) 2000-2008, Intel Corporation. "
                    "Copyright (C) 2015-2024, OpenCV Foundation, all rights reserved."
                    "Thank you to huihut.com for prebuilt MINGW libraries.",
                    "OpenCV",
                    "Computer Vision Library");
    return ver;
}

bool Objdet::isValid(const Class objcls)
{
    return objcls > $nullClass && objcls < $maxClass;
}

Objdet::Class Objdet::objectClass(const QString name)
{
    Class result = $nullClass;
    Objdet tOD;
    const QMetaObject * pQMO = tOD.metaObject();
    const int cCount = pQMO->enumeratorCount();
    int tIndex = 0;
    while (tIndex < cCount && $nullClass == result)
    {
        const QMetaEnum cQME = pQMO->enumerator(tIndex);
        const QString cEnumName(cQME.enumName());
        if ("Class" == cEnumName)
        {
            bool tOK = false;
            int tInt = $nullClass;
            tInt = cQME.keyToValue(qPrintable(name), &tOK);
            if (tOK) result = Class(tInt);
            break;                                      /*v-1-v*/
        }
        ++tIndex;
    }                                                   /*--1--*/
    return result;
}

QString Objdet::className(const Class objcls)
{
    QString result("Null");
    Objdet tOD(objcls);
    if (isValid(objcls))
    {
        const QMetaObject * pQMO = tOD.metaObject();
        const int cCount = pQMO->enumeratorCount();
        int tIndex = 0;
        while (tIndex < cCount && result.isEmpty())
        {
            const QMetaEnum cQME = pQMO->enumerator(tIndex);
            const QString cEnumName(cQME.enumName());
            if ("Class" == cEnumName)
                result =  cQME.valueToKey(objcls);
            ++tIndex;
        }
    }
    return result;
}

bool Objdet::processCascadeClassifier(const bool returnAll)
{
    bool result = false;

    Q_ASSERT(mpCascade);
    if ( ! isDetectorLoaded())
        return result;                                  /*=====*/
    if (inputImage().isNull())
        return result;                                  /*=====*/
    mGreyImage = inputImage().convertedTo(QImage::Format_Grayscale8);
    mGreyMat = cv::Mat(mGreyImage.height(),
                       mGreyImage.width(), CV_8U);
    const int tGreyMatBytes = mGreyMat.total();
    qDebug() << Q_FUNC_INFO << returnAll << mGreyImage.size()
             << mGreyImage.sizeInBytes() << tGreyMatBytes;
    Q_ASSERT(mGreyImage.sizeInBytes() == tGreyMatBytes);
    memcpy(mGreyMat.ptr(0), mGreyImage.bits(), tGreyMatBytes);
    std::vector<cv::Rect> tRectVector;
    std::vector<int> tCountVector;
    std::vector<cv::Rect> tAllRectVector;
    mpCascade->detectMultiScale(mGreyMat, tRectVector, tCountVector,
                                raw().factor(), raw().neighbors(), raw().flags(),
                                raw().cvMinSize(), raw().cvMaxSize());
    if (returnAll)
        mpCascade->detectMultiScale(mGreyMat, tAllRectVector,
                                    raw().factor(), 0, raw().flags(),
                                    raw().cvMinSize(), raw().cvMaxSize());

    result = processResults(tRectVector, tCountVector,
                            tAllRectVector, raw().factor());
    qDebug() << __LINE__ << tRectVector.size()
             << tCountVector.size() << tAllRectVector.size()
             << mOrphanRects.count();
    return result;
}

bool Objdet::processResults(const std::vector<cv::Rect> rects,
                            const std::vector<int> counts,
                            const std::vector<cv::Rect> allrects,
                            const qreal factor)
{
//    const qreal cFactor = raw().factor();
    for (unsigned ix = 0; ix < allrects.size(); ++ix)
    {
        const cv::Rect cCvRect = allrects.at(ix);
        const QRect cRect(cCvRect.tl().x, cCvRect.tl().y,
                          cCvRect.width, cCvRect.height);
        mAllRects.append(cRect);
    }
    QList<SCRect> tRectList = mAllRects;
    QMultiMap<int, DetectorResult> tQualityResultMap;
    for (unsigned ix = 0; ix < rects.size(); ++ix)
    {
        const cv::Rect cCvRect = rects.at(ix);
        const int cCount = counts.at(ix);
        const QRect cResultRect(cCvRect.tl().x, cCvRect.tl().y,
                                cCvRect.width, cCvRect.height);
        const int cQuality
            = calculateQuality(cCount, cResultRect.width(), factor);
        DetectorResult tResult(cResultRect);
        tResult.quality(cQuality);
        tResult.count(cCount);
        tRectList = tResult.takeIncludedRects(tRectList);
        tQualityResultMap.insert( - cQuality, tResult);
        qDebug() << Q_FUNC_INFO << cQuality << cResultRect
                 << tQualityResultMap.count() << tRectList.count();
    }
    mOrphanRects = tRectList;
    mResultList.orphanList(mOrphanRects);
    int tRank = 0;
    foreach (DetectorResult dr, tQualityResultMap.values())
    {
        dr.rank(++tRank);
        mResultList.addRanked(dr);
    }
    qDebug() << Q_FUNC_INFO << mAllRects.count()
             << mOrphanRects.count()
             << mResultList.rankedList().count();
    return true;
}

int Objdet::calculateQuality(const int neighborCount,
                             const int detectWidth,
                             const qreal factor)
{
    int result = 0;
    Q_ASSERT(detectWidth);
    if ( ! qFuzzyCompare(1.100, mRawParms.factor()))
        qWarning() << "Expected factor 1.100";
    result = int((qreal(neighborCount) / qreal(detectWidth))
                 * 500.0 * factor * factor);
    result = qBound(1, result, 999);
    //qDebug() << Q_FUNC_INFO << neighborCount << detectWidth << result;
    return result;
}
