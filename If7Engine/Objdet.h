#pragma once

#include <QObject>

#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QSize>

#include <VersionInfo.h>

#include "DetectorResult.h"
#include "DetectorResultList.h"
#include "ObjdetRawArguments.h"
#include "SCRect.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
class ImageCache;

class Objdet : public QObject
{
    Q_OBJECT
public: // types
    enum Class
    {
        $nullClass = 0,
        FaceFrontal,
        FaceProfileLeft,
        FaceProfileRight,
        Eyes,
        EyeLeft,
        EyeRight,
        EyeBoth,
        $maxClass
    };
    Q_ENUM(Class)
    typedef QList<Class> ClassList;

public: // ctors
    explicit Objdet(QObject *parent = nullptr);
    ~Objdet();
protected:
    Objdet(const Class objcls, QObject * parent=0);

public slots:
    void loadDetectorName(const QString & name) { Q_UNUSED(name); } // TODO
    void loadDetectorXml(const QString & fileName);
    void unloadDetector();

signals:
    void selected(const QFileInfo &fi);
    void error(QString message);

protected:

public: // const
    QString className() const;
    ObjdetRawArguments raw() const { return mRawParms; }
    QImage inputImage(void) const;
    QString performanceString(void) const;
    DetectorResultList resultList(void) const { return mResultList; }
    QList<SCRect> allRects(void) const { return mAllRects; }
    QList<QSize> detectorSizes(void) const;
    bool isDetectorLoaded(void);
    QFileInfo detectorFileInfo() const;
    QString methodString(void);
    QSize sizeFromDetectorXml(const QString & fileName);
    QSize minObjectSize(void) const;
    QSize maxObjectSize(void) const;

public: // non-const
    void set(const ObjdetRawArguments raw);
    void inputImage(const QImage &img);
    void clear();
    bool processCascadeClassifier(const bool returnAll=false);
    bool processResults(const std::vector<cv::Rect> rects,
                        const std::vector<int> counts,
                        const std::vector<cv::Rect> allrects, const qreal factor);
    int calculateQuality(const int neighborCount, const int detectWidth,
                         const qreal factor);
    bool loadXmlCascade(const QString & xmlFilename);

public: // pointers
//    void cache(ImageCache * pc) { mpCache = pc; }
    void cascade(cv::CascadeClassifier * pc) { mpCascade = pc; }
    cv::CascadeClassifier * cascade(void) { return mpCascade; }

public: // static
    static VersionInfo cvVersion();
    static bool isValid(const Objdet::Class objcls);
    static Objdet::Class objectClass(const QString name);
    static QString className(const Objdet::Class objcls);

public:

protected slots:

private:
    QList<SCRect> mAllRects;
    QList<SCRect> mOrphanRects;
    DetectorResultList mResultList;
    const Class cmClass=$nullClass;
    QFileInfo mCascadeFileInfo;
    cv::CascadeClassifier * mpCascade=nullptr;
    ObjdetRawArguments mRawParms;
    //ImageCache * mpCache=nullptr;
    QImage mInputImage;
    QImage mGreyImage;
    cv::Mat mGreyMat;
    //int origScale;
};

inline bool Objdet::isDetectorLoaded()
{
    Q_ASSERT(mpCascade);
    return ! mpCascade->empty();
}

inline QFileInfo Objdet::detectorFileInfo() const
{
    return mCascadeFileInfo;
}

/*
    Q_PROPERTY(QString ClassName READ className)
    Q_PROPERTY(qreal ClassFactor READ classFactor)
    Q_PROPERTY(QString DetectorName READ detectorName)
    Q_PROPERTY(QString DetectorDescription READ detectorDescription)
    Q_PROPERTY(QSize DetectorSize READ detectorSize)
    Q_PROPERTY(int MaxResults READ maxResults WRITE setMaxResults)
    Q_PROPERTY(int MinQuality READ minQuality WRITE setMinQuality)
    Q_PROPERTY(bool ForceFind READ forceFind WRITE setForceFind)
    Q_PROPERTY(qreal Factor READ factor WRITE setFactor)
    Q_PROPERTY(qreal MaxDensity READ maxDensity WRITE setMaxDensity)
    Q_PROPERTY(int MaxDetectors READ maxDetectors WRITE setMaxDetectors)
    Q_PROPERTY(qreal Scale READ scale WRITE setScale)
    Q_PROPERTY(int Flags READ flags WRITE setFlags)
    Q_PROPERTY(int Adjust READ adjust WRITE setAdjust)
    Q_PROPERTY(int MaxAcross READ maxAcross WRITE setMaxAcross)
    Q_PROPERTY(int MinAcross READ minAcross WRITE setMinAcross)
    Q_PROPERTY(int MaxDimension READ maxDimension WRITE setMaxDimension)
    Q_PROPERTY(int MaxPixels READ maxPixels WRITE setMaxPixels)
    Q_PROPERTY(int MinPixels READ minPixels WRITE setMinPixels)
    Q_PROPERTY(int GroupMethod READ groupMethod WRITE setGroupMethod)
    Q_PROPERTY(int GroupThreshold READ groupThreshold WRITE setGroupThreshold)
    Q_PROPERTY(int OverlapThreshold READ overlapThreshold WRITE setOverlapThreshold)
    Q_PROPERTY(int NeighborThreshold READ neighborThreshold WRITE setNeighborThreshold)
    Q_PROPERTY(QString MarkExtraColor READ markExtraColor WRITE setMarkExtraColor)
    Q_PROPERTY(QString MarkScaleColor READ markScaleColor WRITE setMarkScaleColor)
    Q_PROPERTY(QString MarkColors READ markColors WRITE setMarkColors)
    Q_PROPERTY(bool MarkAll READ markAll WRITE setMarkAll)
*/
