#include "IfSearchEngine.h"
#include "version.h"

#include <QtDebug>
#include <QRect>
#include <QSize>
#include <QTimer>

#include "IfSearchApplication.h"
#include "IfSearchWindow.h"
#include "ObjdetEyes.h"
#include "ObjdetFrontal.h"
#include "SCRect.h"

IfSearchEngine::IfSearchEngine(IfSearchApplication *parent)
    : QObject(parent)
    , mpApplication(parent)
{
    setObjectName("IfSearchEngine");
    QTimer::singleShot(100, this, SLOT(init()));
} // c'tor

IfSearchEngine::~IfSearchEngine()
{
}

void IfSearchEngine::init(void)
{
    qDebug() << Q_FUNC_INFO;
    mBaseTimestamp = QDateTime::currentDateTime();
    QTimer::singleShot(100, this, SLOT(start()));
}

void IfSearchEngine::start(void)
{
    qDebug() << Q_FUNC_INFO;
    const QString cDetectorsXmlName("./detectors/Detectors.XML");
    const QString cDetectorClassName("FaceFrontal");
    const QString cDetectorName(""); // blank=default
    if (mpFrontal)
    {
        mpFrontal->unloadDetector();
        mpFrontal->deleteLater();
    }
    mpFrontal = new ObjdetFrontal(this);
    mpFrontal->loadDetectorXml("./detectors/Aim8A001-32-NoSplit.xml");
    if ( ! mpFrontal->isDetectorLoaded())
        qCritical() << "Failed to load frontal:" << mpFrontal->detectorFileInfo();
    if (mpEyes)
    {
        mpEyes->unloadDetector();
        mpEyes->deleteLater();
    }
    mpEyes = new ObjdetEyes(this);
    mpEyes->loadDetectorXml("./detectors/haarcascade_eye.xml");
    if ( ! mpEyes->isDetectorLoaded())
        qCritical() << "Failed to load eyes:" << mpEyes->detectorFileInfo();

    QTimer::singleShot(100, this, SLOT(run()));
} // start()

void IfSearchEngine::run(void)
{
    qDebug() << Q_FUNC_INFO;

    getInputFiles();

    mOutputBaseDir.cd(app()->exeFileInfo().dir().absolutePath());
    QString tOutputBaseDirName = options().baseOutputDir.path();
    tOutputBaseDirName.replace('@', QDateTime::currentDateTime()
                                        .toString("DyyyyMMdd-Thhmm"));
    mOutputBaseDir.mkpath(tOutputBaseDirName);
    if ( ! mOutputBaseDir.cd(tOutputBaseDirName))
        qCritical() << "Can't set base output directory";
    mMarkedDir = mNoFaceDir = mFrontalObjDetDir
        = mDetectedFacesDir = mOutputBaseDir;
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;
    if ( ! mMarkedDir.mkpath(options().markedDir.path()))
        qCritical() << "Can't make marked output directory";
    if ( ! mNoFaceDir.mkpath(options().noFaceDir.path()))
        qCritical() << "Can't make no faces detected output directory";
    if ( ! mDetectedFacesDir.mkpath(options().detectedFacesDir.path()))
        qCritical() << "Can't make detected faces output directory";
    if ( ! mFrontalObjDetDir.mkpath(options().frontalObjdetDir.path()))
        qCritical() << "Can't make objdet output directory";
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;
    if ( ! mMarkedDir.cd(options().markedDir.path()))
        qCritical() << "Can't set marked output directory";
    if ( ! mNoFaceDir.cd(options().noFaceDir.path()))
        qCritical() << "Can't set no faces detected output directory";
    if ( ! mDetectedFacesDir.cd(options().detectedFacesDir.path()))
        qCritical() << "Can't set detected faces output directory";
    if ( ! mFrontalObjDetDir.cd(options().frontalObjdetDir.path()))
        qCritical() << "Can't set objdet output directory";
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;
    app()->win()->clearPixmaps();

    QTimer::singleShot(100, this, SLOT(pulse()));
} // run()


void IfSearchEngine::pulse(void)
{
    qDebug() << Q_FUNC_INFO;

    if (mInputFiles.isEmpty())
    {
        if (options().finishedQuit)
            QTimer::singleShot(100, qApp, SLOT(quit()));
        else if (options().loop)
        {
            if (getInputFiles())
                QTimer::singleShot(options().sampleMsec, this, SLOT(pulse()));
        }
        else if (options().waitingMsec)
            QTimer::singleShot(options().waitingMsec, this, SLOT(pulse()));
        return;
    }
    QFileInfo tInputFile = mInputFiles.takeFirst();
    processFrame(tInputFile);
    QTimer::singleShot(options().sampleMsec, this, SLOT(pulse()));
}

int IfSearchEngine::getInputFiles()
{
    mInputDir.cd(app()->exeFileInfo().dir().absolutePath());
    if ( ! mInputDir.cd(options().inputDir.path()))
        qCritical() << "No input directory at:" << mInputDir.absolutePath();
    static const QStringList scNameFilter = QStringList() << "*.JPG" << "*.PNG";
    mInputFiles = mInputDir.entryInfoList(scNameFilter);
    if (mInputFiles.isEmpty())
        qCritical() << "No input files in:" << mInputDir.absolutePath();
    return mInputFiles.count();
}

void IfSearchEngine::processFrame(const QFileInfo &fi)
{
    qInfo() << Q_FUNC_INFO << fi.baseName();
    const QImage cInputImage = createInputImage(fi);
    if (cInputImage.isNull())
        qCritical() << "Image skipped:" << fi.absoluteFilePath();
    Q_ASSERT(mpFrontal);
    app()->win()->clearPixmaps();
    mpFrontal->clear();
    ObjdetRawArguments tRaw;
    tRaw.factor(1.100), tRaw.neighbors(3), tRaw.flags(0),
        tRaw.set(ObjdetRawArguments::ForceRaw),
        tRaw.minSize(QSize()), tRaw.maxSize(QSize()),
        tRaw.inputSize(cInputImage.size());
    mpFrontal->inputImage(cInputImage);
    mpFrontal->set(tRaw);
    if ( ! mpFrontal->processCascadeClassifier(true))
        qCritical() << "ObjDet failed:" << fi.absoluteFilePath();
    mResults = mpFrontal->resultList();
    QImage tMarkedImage = mpFrontal->markedImage(500);
    const QFileInfo tMarkedFI(mMarkedDir, fi.baseName() + ".png");
    if (tMarkedImage.save(tMarkedFI.absoluteFilePath()))
        qInfo() << tMarkedFI.absoluteFilePath() << tMarkedImage;
    QImage tDetectImage = mpFrontal->detectImage(500);
    const QFileInfo tDetectFI(mFrontalObjDetDir, fi.baseName() + ".png");
    if (tDetectImage.save(tDetectFI.absoluteFilePath()))
        qInfo() << tDetectFI.absoluteFilePath() << tDetectImage;
    app()->win()->clearFacePixmaps();
    app()->win()->setMarked(tMarkedImage);
    app()->win()->setDetect(tDetectImage);
    if (mResults.count() == 0)
    {
        const QFileInfo tNoFaceFI(mNoFaceDir, fi.baseName() + ".png");
        if (tMarkedImage.save(tNoFaceFI.absoluteFilePath()))
            qInfo() << tNoFaceFI.absoluteFilePath() << tMarkedImage;
        app()->win()->clearFacePixmaps();
    }
    processFaces(cInputImage, fi, options().minQuality);
    if (options().deleteAfter)
    {
        QFile tInputFile(fi.filePath());
        tInputFile.remove();
        qDebug() << fi.absoluteFilePath() << "removed";
    }

}

QImage IfSearchEngine::createInputImage(const QFileInfo &fi)
{
    QImage result;
    const QImage cInput(fi.filePath());
    const QSize cInputSize(cInput.width() & 0xFFF0, cInput.height() & 0xFFF0);
    const QRect cInputRect(QPoint((cInput.width() - cInputSize.width()) / 2,
                                  (cInput.height() - cInputSize.height()) / 2),
                           cInputSize);
    result = cInput.copy(cInputRect).convertedTo(QImage::Format_ARGB32);
    if (result.isNull())
        qWarning() << "Null input image" << cInput << result;
    return result;
}

void IfSearchEngine::processFaces(const QImage &inputImage,
                                  const QFileInfo &inputFI,
                                  const int minQuality)
{
    qInfo() << Q_FUNC_INFO << mResults.count();
    app()->win()->clearFacePixmaps();
    foreach (const DetectorResult cResult, mResults.rankedList())
    {
        const int cQuality = cResult.quality();
        const SCRect cDetectRect = cResult.rect();
        SCRect tCropRect = (cDetectRect * 1.25).trimmed(16);
        const int cRank = cResult.rank();
        const QImage cFaceImage = inputImage.copy(tCropRect);
        if (cQuality < minQuality)  continue;               /*-----*/
        findEyes(cRank, inputImage, cResult);
        const QString cFaceFileName
            = QString("./Q%1/#%2q%3x%4y%5w%6e%7-%8.png")
                  .arg(cQuality/100*100, 3, 10, QChar('0'))     // 1
                  .arg(cRank, 2, 10, QChar('0'))                // 2
                  .arg(cQuality, 3, 10, QChar('0'))             // 3
                  .arg(cDetectRect.x(), 4, 10, QChar('0'))      // 4
                  .arg(cDetectRect.y(), 4, 10, QChar('0'))      // 5
                  .arg(cDetectRect.width(), 3, 10,QChar('0'))   // 6
                  .arg(0, 3, 10, QChar('0'))                    // 7
                  .arg(inputFI.baseName());                     // 8
        const QFileInfo cFaceFI(mDetectedFacesDir, cFaceFileName);
        cFaceFI.dir().mkpath(".");
        qInfo() << cFaceFI.absoluteFilePath()
                << cFaceImage.save(cFaceFI.filePath(), "PNG", 90);
        app()->win()->appendFace(cFaceImage);
    }

}

void IfSearchEngine::findEyes(const int ix1, const QImage &frameImage,
                              const DetectorResult cFaceResult)
{

}
