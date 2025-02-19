#pragma once

#include <QObject>

#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QSize>

#include <VersionInfo.h>

#include "DetectorResult.h"

//#include <opencv2/opencv.hpp>
//#include <opencv2/objdetect.hpp>
#include "C:\code\bin\DbgOpenCV-v4.10.0\include/opencv2/opencv.hpp"
#include "C:\code\bin\DbgOpenCV-v4.10.0\include/opencv2/objdetect.hpp"
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
        EyeEither,
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
//    Objdet(const QString & classname, QObject * parent=0);

public slots:
    void loadDetectorName(const QString & name) {;} // TODO
    void loadDetectorXml(const QString & fileName);
    void unloadDetector();

signals:
    void selected(const QFileInfo &fi);
    void error(QString message);


public: // const
    QString className() const;
    void clear(void);
    void setImage(QImage img);
    void setImage(const QString & ImageId);
    QImage image(void) const;
    QImage detectImage(void) const;

    QString performanceString(void) const;
    QList<DetectorResult> getResults(void) const { return results; }
    QList<QRect> getAllObjects(void) const { return allObjects; }
    QList<QSize> detectorSizes(void) const;
    bool isDetectorLoaded(void);
    QString methodString(void);
    QSize sizeFromXml(const QString & fileName);
    QSize minObjectSize(void) const;
    QSize maxObjectSize(void) const;

public: // non-const


public: // pointers
    void cache(ImageCache * pc) { mpCache = pc; }
    void cascade(cv::CascadeClassifier * pc) { mpCascade = pc; }
    cv::CascadeClassifier * cascade(void) { return mpCascade; }

public: // static
    static VersionInfo cvVersion();
    static bool isValid(const Objdet::Class objcls);
    static Objdet::Class objectClass(const QString name);
    static QString className(const Objdet::Class objcls);

protected:
    bool processCascadeClassifier(bool returnAll=false);
    bool loadXmlCascade(const QString & xmlFilename);
    void handleResults(bool returnAll=false);

protected slots:
//    bool process(bool returnAll=false);

protected:
    QList<QRect> allObjects;
    QList<DetectorResult>  results;
    QMultiMap<double, DetectorResult> allResults;

private:
    const Class cmClass=$nullClass;
    cv::CascadeClassifier * mpCascade=nullptr;
    ImageCache * mpCache=nullptr;
    QImage imgOrig;
    int origScale;
};

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
