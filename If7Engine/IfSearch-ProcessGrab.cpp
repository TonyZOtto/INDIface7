#include "IfSearch.h"

#include <QBitmap>
#include <QPixmap>
#include <QStringList>
#include <QTimer>

#include <EigenFace.h>
#include <EigenFaceGenerator.h>
#include <EigenFaceSearcher.h>
#include <EigenFaceParameters.h>
#include <EigenFaceTemplate.h>
#include <FaceBase.h>

#include <Eyes.h>
#include <FileWriteProfile.h>
#include <ImageCache.h>
#include <ImageMarker.h>
#include <ImageSource.h>
#include <InputHotdir.h>
#include <QQRect.h>
#include <Return.h>
#include <Setting.h>
#include <Settings.h>

#include <HeightGrid.h>
#include <SkinDetector.h>
#include <SkinMatcher.h>
#include <CharacteristicColor.h>
#include <ClothesMatchProperties.h>
#include <ClothesMatcher.h>



void IfSearch::processGrab(void)
{
    QList<DetectorResult> resultList;

    if (paused
            || ffdBusy
            || imageCache.isGrabEmpty()
            || ! pendingFaces.isEmpty()
            || ! commandMode.isEmpty())
    {
        if (writer)
            writer->pumpAll();
        if (ffdBusy)
            qInfo("processGrab() FFD is busy");
        if (paused)
            qInfo("processGrab() paused");
        if (imageCache.isGrabEmpty())
        {
            appSettings->setValue("Input/Processing", "done");
            qInfo("processGrab() empty grab cache");
        }
        if (pendingFaces.isEmpty())
            qInfo("processGrab() has no pending faces");
        if ( ! commandMode.isEmpty())
            qInfo() << "Busy with" << commandMode;
        return;
    }

    bool detectEnabled = optDetectEnable->toBool();
    if (detectEnabled && ! ffd->hasDetector())
    {
        qWarning("No Facial Detector Selected");
        detectEnabled = false;
    }

    qreal inputOverCrop = optInputOverCrop->toInt()
                          ? 100.0 / (qreal)optInputOverCrop->toInt() : 0.0;
    QString imageId = imageCache.takeGrab();
    QImage image = imageCache.getImage(imageId);
    QDateTime grabTime = imageCache.timeStamp(imageId);
    if (streamBase_mst.isNull())
        streamBase_mst.set(grabTime);

    idGenerator.reset();
    idGenerator.setFrameFormat(appSettings->value("Output/FrameIdFormat",
                                        "%i").toString());
    idGenerator.setFaceFormat(appSettings->value("Output/FaceIdFormat",
                                        "%i-%x%y%q%w%c%e%m").toString());
    idGenerator.setFrameId(imageId);
    idGenerator.setFrameMsd(streamBase_mst.delta(grabTime));
    idGenerator.setModifiedMst(grabTime.toMSecsSinceEpoch());
    idGenerator.setFrameNumber(1+FramesProcessed);
    idGenerator.setFileName(imageCache.fileName(imageId));

    QImage imageRgb;
    imageCache.hold(imageId, ffd);
    if (frameStatistics)
        frameStatistics->start(grabTime, imageCache.grabSize(), image.size());

    if (camera)
        imageCache.release(imageId, camera);
    if (hotdir)
        imageCache.release(imageId, hotdir);

    if (image.isNull())
    {
        qWarning() << "***Null Image: " << imageId;
        if ( ! imageCache.isGrabEmpty())
            QTimer::singleShot(10, this, SLOT(processGrab()));
        imageCache.release(imageId);
        appSettings->setValue("Output/NullFrames", ++NullFrames);
        return;
    }

    qInfo() << QString("===Processing %2\t%1 modified %3 [%4]")
              .arg(imageId).arg(1+FramesProcessed)
             .arg(grabTime.toString("yyyyMMdd hhmmsszzz"))
             .arg(grabTime.toMSecsSinceEpoch());
    appSettings->setValue("Input/Processing", imageId);

    int maxDim = appSettings->value("Input/MaxDimension", 1536).toInt();
    if (xformProps.dimension(image.size()) > maxDim)
    {
        qreal xfScale = qIsNull(xformProps.getScale())
                ? 1.0 : xformProps.getScale();
        int dim = qMax(image.width(), image.height());
        int dimDiv = 1 + dim / maxDim;
        if (xfScale > (1.0 / (qreal)dimDiv))
            xformProps.setScale(1.0 / (qreal)dimDiv);
    }

    if ( ! xformProps.isNull())
    {
        image = image.transformed(xformProps.transform());
    }

    if (detectEnabled)
    {
        ffdBusy = true;
        ffd->setImage(image);
        if ( ! ffd->process(&resultList))
        {
            qCritical() << "Error in Face Detector for " << imageId;
            resultList.clear();
        }
        else
        {
            if (frameStatistics)
                frameStatistics->detected(resultList.size(),
                                          ffd->raw(),
                                          ffd->strange(),
                                          ffd->msec());
        }
    }
    else
    {
        resultList.clear();
        if ( ! qIsNull(inputOverCrop) && optGenerateEnable->toBool())
        {
            QRect head = QQRect(image.size() * inputOverCrop,
                               image.rect().center());
            DetectorResult result(1, 0, head);
            resultList.append(result);
        }
    }

    if ( ! resultList.isEmpty())
        idGenerator.setQuality(resultList.last().score());

    if (fwpCharcol->isActive())
    {
        if (imageRgb.isNull())
            imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QImage charcolImage = CharacteristicColor::process(imageRgb);
        fwpCharcol->write(charcolImage, idGenerator.frame("Charcol"));
    }
    if (fwpSkin->isActive())
    {
        if (imageRgb.isNull())
            imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QImage skinImage = skinDetector->masked(imageRgb);
        fwpSkin->write(skinImage, idGenerator.frame("Skin"));
    }

    if ( ! resultList.isEmpty())
        idGenerator.setBestQuality(resultList.first().score());
    if (fwpDetect->isActive() && detectEnabled)
        fwpDetect->write(ffd->detectImage(), idGenerator.frame("Detect"));

    if (resultList.isEmpty())
    {
        ffd->clear();
        ffdBusy = false;
        if (pausePending)
        {
            pausePending = false;
            paused = true;
            imageCache.flushGrab();
            pendingFaces.clear();
            qInfo("---PAUSED---");
        }
        qInfo() << "^^^No faces in " << imageId;
        fwpNoFace->write(image, idGenerator.frame("NoFace"));
        fwpCapture->write(imageId);
        fwpCapture2->write(image, idGenerator.frame("Input"));
        if (fpWriter && framePerformance && frameStatistics)
        {
            frameStatistics->finish(0, imageCache.status());
            framePerformance->calculate(frameStatistics, imageId);
            fpWriter->writeLine();
        }

        if (fwpHeight->isActive() && optForceHeight->toBool())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            Q_ASSERT(heightGrid);
            heightGrid->clear();
            QImage heightImage(heightGrid->gridImage(imageRgb));
            fwpHeight->write(heightImage, idGenerator.face("Height"));
        }

        if (optForceMarked->toBool() && fwpMarked->isActive())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            markedImage = imageRgb;
            ImageMarker marker(&markedImage);

            // background image?
            QString bgFileName = optMarkBackgroundFile->toString();
            if ( ! bgFileName.isEmpty())
            {
                QImage bgImage(bgFileName);
                bgImage = bgImage.scaled(markedImage.size(), Qt::KeepAspectRatioByExpanding);
                marker.drawImage(QPoint(0,0), bgImage);
            }

            // background color?
            QColor bgColor = optMarkBackgroundColor->value<QColor>();
            if (bgColor.isValid())
            {
                bgColor.setAlpha(255.0 * optMarkBackgroundTransparency->toDouble() / 100.0);
                marker.fillRect(markedImage.rect(), bgColor);
            }

            marker.end();
            fwpMarked->write(markedImage, idGenerator.frame("Marked"));
        }
        appSettings->setValue("Output/FramesProcessed",
                              QString::number(++FramesProcessed));
    }
    else
    {
        imageCache.hold(imageId, eigenFace);

        // --- Start Marked Image if markedDir is active
        isHeight = false;
        if (fwpMarked->isActive())
        {
            if (imageRgb.isNull())
                imageRgb = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            markedImage = imageRgb;
            ImageMarker marker(&markedImage);
            isMarked = false;

            // background image?
            QString bgFileName = optMarkBackgroundFile->toString();
            if ( ! bgFileName.isEmpty())
            {
                QImage bgImage(bgFileName);
                bgImage = bgImage.scaled(markedImage.size(), Qt::KeepAspectRatioByExpanding);
                marker.drawImage(QPoint(0,0), bgImage);
            }

            // background color?
            QColor bgColor = optMarkBackgroundColor->value<QColor>();
            if (bgColor.isValid())
            {
                bgColor.setAlpha(255.0 * optMarkBackgroundTransparency->toDouble() / 100.0);
                marker.fillRect(markedImage.rect(), bgColor);
            }
        }

        // setup each face for processing
        //  (starting with the lowest quality so high quality
        //   faces are last marked and therefor on top)
        while ( ! resultList.isEmpty())
        {
            DetectorResult result = resultList.takeLast();
            pendingFaces.append(QPair<QString,DetectorResult>(imageId, result));
            if (pendingFaces.isEmpty())
                QTimer::singleShot(10, this, SLOT(processFace()));
        }
        // append a blank face as signal to write marked image
        pendingFaces.append(QPair<QString,DetectorResult>(imageId, DetectorResult()));
    }

    grabImage = image;
    imageCache.release(imageId, ffd);

    if ( ! imageCache.isGrabEmpty())
        QTimer::singleShot(10, this, SLOT(processGrab()));
} // processGrab()
