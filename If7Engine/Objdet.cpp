#include "Objdet.h"

#include <QMetaEnum>
#include <QMetaObject>

#include <opencv2/opencv.hpp>

#include "ObjectHelper.h"

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
#if 1
    const std::__cxx11::basic_string<char,
                                     std::char_traits<char>,
                                     std::allocator<char> >
        cStdCascadeXmlName = tFI.absoluteFilePath().toStdString();
#else
    std::__1::basic_string<char,
                           std::__1::char_traits<char>,
                           std::__1::allocator<char> > const&
        = cStdCascadeXmlName = tFI.absoluteFilePath().toStdString();
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

void Objdet::setImage(const QImage &inputImage)
{
    qDebug() << Q_FUNC_INFO << inputImage;
    if (inputImage.isGrayscale())
        Q_ASSERT(QImage::Format_Grayscale8 == inputImage.format());
    else
        Q_ASSERT(QImage::Format_RGB32 == inputImage.format());
    mInputImage = inputImage;
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
    const QImage cInputGreyImage = inputImage().convertedTo(QImage::Format_Grayscale8);
    cv::Mat tGreyMat (cInputGreyImage.height(), cInputGreyImage.width(), CV_8UC1);
    const int tGreyMatBytes = tGreyMat.checkVector(1, 1, true);
    qDebug() << Q_FUNC_INFO << returnAll << tGreyMatBytes;
    Q_ASSERT(cInputGreyImage.bytesPerLine() * cInputGreyImage.width()
             == tGreyMatBytes);
    memcpy(tGreyMat.ptr(0), cInputGreyImage.bits(), tGreyMatBytes);
    std::vector<cv::Rect> tRectVector;
    std::vector<int> tCountVector;
    std::vector<cv::Rect> tAllRectVector;
    mpCascade->detectMultiScale(tGreyMat, tRectVector, tCountVector,
                                raw().Factor(), raw().Neighbors(), raw().Flags(),
                                raw().cvMinSize(), raw().cvMaxSize());
    if (returnAll)
        mpCascade->detectMultiScale(tGreyMat, tAllRectVector,
                                    raw().Factor(), 0, raw().Flags(),
                                    raw().cvMinSize(), raw().cvMaxSize());

    result = processResults(tRectVector, tCountVector, tAllRectVector);
    return result;
}

bool Objdet::processResults(const std::vector<cv::Rect> rects,
                            const std::vector<int> counts,
                            const std::vector<cv::Rect> allrects)
{
    bool result = rects.size() != 0 && counts.size() != 0
                  && rects.size() == counts.size();
    if ( ! result) return result;                               /*=====*/
    const qreal cFactor = raw().Factor();
    for (int ix = 0; ix < rects.size(); ++ix)
    {
        const cv::Rect cCvRect = rects.at(ix);
        const int cCount = counts.at(ix);
        const QRect cResultRect(cCvRect.tl().x, cCvRect.tl().y,
                                cCvRect.width, cCvRect.height);
        DetectorResult tResult(cResultRect);
    }
    return result;
}
/*
QString ObjdetCatalog::className(const Objdet::Class objcls) // static
{
    Objdet od(objcls);
    const ObjectHelper cOH(od);
    return cOH.enumKey("Class", objcls);
}
*/
