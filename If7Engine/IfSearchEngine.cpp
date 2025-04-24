#include "IfSearchEngine.h"
#include "version.h"

#include <QtDebug>
#include <QPainter>
#include <QPen>
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
    if (mpFrontal)
    {
        mpFrontal->unloadDetector();
        mpFrontal->deleteLater();
    }
    mpFrontal = new ObjdetFrontal(this);
    mpFrontal->loadDetectorXml("./detectors/Aim8A001-32-NoSplit.xml");
    if ( ! mpFrontal->isDetectorLoaded())
        qCritical() << "Failed to load frontal:" << mpFrontal->detectorFileInfo();
    if (mpLEyes)
    {
        mpLEyes->unloadDetector();
        mpLEyes->deleteLater();
    }
    if (mpREyes)
    {
        mpREyes->unloadDetector();
        mpREyes->deleteLater();
    }
    mpLEyes = new ObjdetEyes(Objdet::EyeLeft, this);
    mpLEyes->loadDetectorXml("./detectors/haarcascade_eye.xml");
    if ( ! mpLEyes->isDetectorLoaded())
        qCritical() << "Failed to load left eyes:" << mpLEyes->detectorFileInfo();
    mpREyes = new ObjdetEyes(Objdet::EyeRight, this);
    mpREyes->loadDetectorXml("./detectors/haarcascade_eye.xml");
    if ( ! mpREyes->isDetectorLoaded())
        qCritical() << "Failed to load right eyes:" << mpREyes->detectorFileInfo();

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
    mMarkedDir = mNoFaceDir = mFrontalObjdetDir = mEyesObjdetDir
        = mDetectedFacesDir = mOutputBaseDir;
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;
    if ( ! mMarkedDir.mkpath(options().markedDir.path()))
        qCritical() << "Can't make marked output directory";
    if ( ! mNoFaceDir.mkpath(options().noFaceDir.path()))
        qCritical() << "Can't make no faces detected output directory";
    if ( ! mDetectedFacesDir.mkpath(options().detectedFacesDir.path()))
        qCritical() << "Can't make detected faces output directory";
    if ( ! mFrontalObjdetDir.mkpath(options().frontalObjdetDir.path()))
        qCritical() << "Can't make frontal objdet output directory";
    if ( ! mEyesObjdetDir.mkpath(options().eyesObjdetDir.path()))
        qCritical() << "Can't make eyes objdet output directory";
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;
    if ( ! mMarkedDir.cd(options().markedDir.path()))
        qCritical() << "Can't set marked output directory";
    if ( ! mNoFaceDir.cd(options().noFaceDir.path()))
        qCritical() << "Can't set no faces detected output directory";
    if ( ! mDetectedFacesDir.cd(options().detectedFacesDir.path()))
        qCritical() << "Can't set detected faces output directory";
    if ( ! mFrontalObjdetDir.cd(options().frontalObjdetDir.path()))
        qCritical() << "Can't set objdet output directory";
    if ( ! mEyesObjdetDir.cd(options().eyesObjdetDir.path()))
        qCritical() << "Can't set objdet output directory";
    qDebug() << mOutputBaseDir << mMarkedDir << mDetectedFacesDir;

    QTimer::singleShot(1000, this, SLOT(pulse()));
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
    const int cMinQuality = options().minQuality;
    const QImage cInputImage = createInputImage(fi);
    if (cInputImage.isNull())
        qCritical() << "Image skipped:" << fi.absoluteFilePath();
    Q_ASSERT(mpFrontal);
    app()->win()->clearPixmaps();
    app()->win()->clearFacePixmaps();
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
    mFaceResults = mpFrontal->resultList();
    QImage tMarkedImage = mpFrontal->markedImage(cMinQuality);
    const QFileInfo tMarkedFI(mMarkedDir, fi.baseName() + ".png");
    if (tMarkedImage.save(tMarkedFI.absoluteFilePath()))
        qInfo() << tMarkedFI.absoluteFilePath() << tMarkedImage;
    QImage tDetectImage = mpFrontal->detectImage(cMinQuality);
    const QFileInfo tDetectFI(mFrontalObjdetDir, fi.baseName() + ".png");
    if (tDetectImage.save(tDetectFI.absoluteFilePath()))
        qInfo() << tDetectFI.absoluteFilePath() << tDetectImage;
    app()->win()->clearFacePixmaps();
    app()->win()->setMarked(tMarkedImage);
    app()->win()->setDetect(tDetectImage);
    if (mFaceResults.count(cMinQuality) == 0)
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
    qInfo() << Q_FUNC_INFO << mFaceResults.count();
    app()->win()->clearFacePixmaps();
    foreach (const DetectorResult cResult, mFaceResults.rankedList())
    {
        const int cQuality = cResult.quality();
        if (cQuality >= minQuality)
        {
            const QImage cFaceImage
                = writeFaceImage(inputFI, inputImage, cResult);
#ifdef EYEFIND
            findEyes(cFaceImage, inputFI, cResult);
#endif
        }
    }
}

QImage IfSearchEngine::writeFaceImage(const QFileInfo inputFI,
                                      const QImage &inputImage,
                                      const DetectorResult faceResult)
{
    const SCRect cDetectRect = faceResult.rect();
    SCRect tCropRect = (cDetectRect * 1.25).trimmed(16);
    qInfo() << Q_FUNC_INFO << inputFI.baseName() << inputImage.size()
            << faceResult.rect().toDebugString()
            << tCropRect.toDebugString();
    const int cRank = faceResult.rank();
    const int cQuality = faceResult.quality();
    const QImage cFaceImage = inputImage.copy(tCropRect);
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
    app()->win()->appendFace(cFaceImage.scaled(IfSearchWindow::faceThumbSize()));
    return cFaceImage;
}

void IfSearchEngine::findEyes(const QImage &frameImage,
                              const QFileInfo inputFI,
                              const DetectorResult faceResult)
{
    const SCRect cFaceRect = faceResult.rect();
    SCRect tLRoi(cFaceRect.size() * 0.5,
                 QPoint(cFaceRect.left()  + cFaceRect.width()  / 2,
                        cFaceRect.top()   +  cFaceRect.height() / 2));
    SCRect tRRoi(cFaceRect.size() * 0.5,
                 QPoint(cFaceRect.right() - cFaceRect.width()  / 2,
                        cFaceRect.top()   + cFaceRect.height() / 2));
    tLRoi &= SCRect(frameImage.rect());
    tRRoi &= SCRect(frameImage.rect());
    QImage tLEyeImage = frameImage.copy(tLRoi.toQRect());
    QImage tREyeImage = frameImage.copy(tRRoi.toQRect());
    int tLEyeScale = (tLEyeImage.width() >= 96) ? 1
                         : qRound(96 / tLEyeImage.width() + 0.999);
    int tREyeScale = (tREyeImage.width() >= 96) ? 1
                         : qRound(96 / tREyeImage.width() + 0.999);
    tLEyeImage = tLEyeImage.scaledToWidth(tLEyeImage.width() * tLEyeScale);
    tREyeImage = tREyeImage.scaledToWidth(tREyeImage.width() * tREyeScale);
    const DetectorResultList cLEyeResults
        = findEye(Objdet::EyeLeft,  tLEyeImage, tLRoi, tLEyeScale);
    const DetectorResultList cREyeResults
        = findEye(Objdet::EyeRight, tREyeImage, tRRoi, tREyeScale);
    mLEyeResults.append(cLEyeResults);
    mREyeResults.append(cREyeResults);
    const QImage cLEyeImage
        = writeEyeImage(false, inputFI, tLEyeImage, faceResult, cLEyeResults);
    const QImage cREyeImage
        = writeEyeImage(true,  inputFI, tREyeImage, faceResult, cREyeResults);
    app()->win()->appendEyes(cLEyeImage, cREyeImage);
}

DetectorResultList IfSearchEngine::findEye(const Objdet::Class objClass,
                                            const QImage &eyeImage,
                                            const SCRect eyeRoi,
                                            const int eyeScale)
{
    qInfo() << Q_FUNC_INFO << objClass << eyeRoi << eyeScale;
    DetectorResultList result;
    ObjdetEyes * pEyes = (objClass == Objdet::EyeLeft)
                            ? mpLEyes : mpREyes;
    Q_ASSERT(pEyes);
    Q_ASSERT(pEyes->isDetectorLoaded());
    ObjdetRawArguments tRaw;
    tRaw.factor(1.100), tRaw.neighbors(3), tRaw.flags(0),
        tRaw.set(ObjdetRawArguments::ForceRaw),
        tRaw.minSize(QSize()), tRaw.maxSize(QSize()),
        tRaw.inputSize(eyeImage.size());
    pEyes->inputImage(eyeImage);
    pEyes->set(tRaw);
    if ( ! pEyes->processCascadeClassifier(true))
        qCritical() << "ObjDet eyes failed";
    result = pEyes->resultList();
    result.adjustRanked(eyeRoi, eyeScale);
    return result;
}

QImage IfSearchEngine::writeEyeImage(const bool isRight,
                                     const QFileInfo inputFI,
                                     const QImage eyeImage,
                                     const DetectorResult faceDR,
                                     const DetectorResultList eyeDRL)
{
    static const int scThumbWidth = IfSearchWindow::faceThumbSize().width();
    QImage result = eyeImage.scaledToWidth(scThumbWidth / 2);
    const qreal cScaleF = qreal(scThumbWidth) / qreal(eyeImage.width());
    qInfo() << Q_FUNC_INFO << isRight << inputFI.baseName() << eyeImage.size()
            << faceDR.toDebugStrings() << eyeDRL.best().toDebugStrings();
    QPainter tPainter;
    tPainter.begin(&result);
    foreach (const DetectorResult cDR, eyeDRL.rankedList())
    {
        const SCRect cRect = cDR.rect();
        QPen tPen(cDR.eyeQualityColor());
        tPainter.setPen(tPen);
        tPainter.drawRect((cRect * cScaleF).toQRect());
    }
    tPainter.end();
    const QString cEyeFileName
        = QString("./#%1%2eye-%3.png")
              .arg(faceDR.rank(), 2, 10, QChar('0'))                // 1
              .arg(isRight ? "R" : "L")                             // 2
              .arg(inputFI.baseName());                             // 3
    const QFileInfo cEyeFI(mEyesObjdetDir, cEyeFileName);
    qInfo() << cEyeFI.absoluteFilePath()
            << result.save(cEyeFI.filePath(), "png", 90);
    return result;
}
