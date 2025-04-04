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
#include "VersionInfo.h"

class ObjdetCatalog;
class ObjdetEyes;
class ObjdetFrontal;

class IfSearchEngine : public QObject
{
    Q_OBJECT

public:
    IfSearchEngine(IfSearchApplication * parent=nullptr);
    ~IfSearchEngine();

private:
    void processFrame(const QFileInfo &fi);
    QImage createInputImage(const QFileInfo &fi);
    void processFaces(const QImage &inputImage,
                      const QFileInfo &inputFI,
                      const int minQuality=500);
    void findEyes(const int ix1, const QImage &frameImage,
                  const DetectorResult cFaceResult);
    IfSearchApplication * app() const;
    IfSearchApplication::Options options() const;

private slots:
    void init(void);
    void start(void);
    void run(void);
    void pulse(void);
    int getInputFiles();

private:
    IfSearchApplication * mpApplication=nullptr;
    ObjdetCatalog * mpObjdetCatalog=nullptr;
    ObjdetFrontal * mpFrontal=nullptr;
    ObjdetEyes * mpEyes=nullptr;
    QDateTime mBaseTimestamp;
    QDir mInputDir;
    QDir mOutputBaseDir;
    QDir mMarkedDir;
    QDir mNoFaceDir;
    QDir mDetectedFacesDir;
    QDir mFrontalObjDetDir;
    QDir mEyesObjDetDir;
    QFileInfoList mInputFiles;
    DetectorResultList mResults;

private:

private:
};

inline IfSearchApplication *IfSearchEngine::app() const {  Q_ASSERT(mpApplication); return mpApplication; }
inline IfSearchApplication::Options IfSearchEngine::options() const { return app()->options(); }


