#include "ObjdetEyes.h"

#include "IfSearchWindow.h"
/*
ObjdetEyes::ObjdetEyes(QObject *parent)
    : Objdet{Objdet::Eyes, parent}
{
    setObjectName("ObjdetEyes:Null");
}
*/
ObjdetEyes::ObjdetEyes(const Class eye, QObject *parent)
    : Objdet{eye, parent}
{
    setObjectName("ObjdetEyes:" + (EyeRight == cmClass) ? "Right" : "Left");
}

ObjdetEyes::ObjdetEyes(const Objdet::Class eye,
                       const QImage &frame,
                       const DetectorResult &faceDR,
                       QObject *parent)
    : Objdet{eye, parent}
    , mInputFrame(frame)
    , mFaceResult(faceDR)
{
    setObjectName("ObjdetEyes:" + (EyeRight == cmClass) ? "Right" : "Left");
}

void ObjdetEyes::detectEye()
{
    clear();
    calculateEyeRoi(); // TODO Parm EyeOverCrop
    generateEyeImage();

}

SCRect ObjdetEyes::calculateEyeRoi(const unsigned overCrop)
{
    SCRect result;
    const SCRect cFaceRect = mFaceResult.rect();
    const QPoint cEyeRoiCenter
        = QPoint(cFaceRect.x() + cFaceRect.width() / 2 * isRight() ? +1 : -1,
                 cFaceRect.y() - cFaceRect.height() / 2);
    result = SCRect(cFaceRect.size() / 2, cEyeRoiCenter);
    if (overCrop) result *= 1.0 + (qreal(overCrop) / 100.0);
    result &= mInputFrame.rect();
    return mFaceEyeRect = result;
}

void ObjdetEyes::generateEyeImage()
{
    const int cThumbWidth = IfSearchWindow::faceThumbSize().width();
    mDetectScale = (mFaceEyeRect.width() > cThumbWidth) ? 1
        : int(0.999 + qreal(cThumbWidth) / qreal(mFaceEyeRect.width()));
    mFaceEyeRect *= mDetectScale;
    mFaceEyeRect = mFaceEyeRect.trimmed(16);
    mEyeImage = mInputFrame.scaledToWidth(mInputFrame.width() * mDetectScale)
                           .copy(mFaceEyeRect);
    qInfo() << Q_FUNC_INFO << mInputFrame.size() << mDetectScale
            << mFaceEyeRect.toDebugString() << mEyeImage.size();
    Objdet::inputImage(mEyeImage);
}
