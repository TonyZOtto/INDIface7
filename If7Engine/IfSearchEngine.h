// @file IfSearch.h
#pragma once

#include <QObject>

#include <QDateTime>
#include <QDir>
#include <QFileInfoList>
#include <QImage>
#include <QList>

#include "DetectorResultList.h"
#include "IfSearchApplication.h"
#include "Objdet.h"
#include "VersionInfo.h"

class ObjdetCatalog;
class ObjdetEyes;
class ObjdetFrontal;

class IfSearchEngine : public QObject
{
    Q_OBJECT

public: // ctors
    IfSearchEngine(IfSearchApplication * parent=nullptr);
    ~IfSearchEngine();

public: // const

public: // non-const

public: // pointers
    IfSearchApplication * app();


private:
    void processFrame(const QFileInfo &fi);
    void processFaces();
    int scanInputDirectory();
    QImage readInputImage(const QFileInfo &fi);
    void writeFrameImages();
    SCRect calculateFaceRect(const DetectorResult faceDR);
    QImage writeFaceImage(const QFileInfo inputFI,
                          const QImage &inputImage,
                          const DetectorResult faceResult);
    void findEyes(const QImage &frameImage,
                  const QFileInfo inputFI,
                  const DetectorResult faceResult);
    DetectorResultList findEye(const Objdet::Class objClass,
                               const QImage &eyeImage,
                               const SCRect eyeRoi,
                               const int eyeScale);
    QImage writeEyeImage(const bool isRight,
                         const QFileInfo inputFI,
                         const QImage eyeImage,
                         const DetectorResult faceDR,
                         const DetectorResultList eyeDRL);
    IfSearchApplication * app() const;
    IfSearchApplication::Options options() const;

private slots:
    void init(void);
    void start(void);
    void startFrontal(void);
    void startEyes(void);
    void run(void);
    void pulse(void);

private:
    IfSearchApplication * mpApplication=nullptr;
    ObjdetCatalog * mpObjdetCatalog=nullptr;
    ObjdetFrontal * mpFrontal=nullptr;
    ObjdetEyes * mpLEyes=nullptr;
    ObjdetEyes * mpREyes=nullptr;
    QDateTime mBaseTimestamp;
    QDir mInputDir;
    QDir mOutputBaseDir;
    QDir mMarkedDir;
    QDir mNoFaceDir;
    QDir mDetectedFacesDir;
    QDir mFrontalObjdetDir;
    QDir mEyesObjdetDir;
    QFileInfoList mInputFileList;
    // current frame
    QFileInfo mCurrentFrameFI;
    QImage mCurrentFrameImage;
    QImage mCurrentMarkedImage;
    QImage mCurrentDetectImage;
    DetectorResultList mFrameFaceResults;
    // current face
    DetectorResult mCurrentFaceDR;
    SCRect mCurrentFaceRect;
    DetectorResultList mCurrentFaceLEyeDRL;
    DetectorResultList mCurrentFaceREyeDRL;

private:

private:
};

inline IfSearchApplication *IfSearchEngine::app() { return mpApplication; }
inline IfSearchApplication *IfSearchEngine::app() const {  Q_ASSERT(mpApplication); return mpApplication; }
inline IfSearchApplication::Options IfSearchEngine::options() const { return app()->options(); }


