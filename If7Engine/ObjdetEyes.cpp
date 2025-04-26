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
    generateEyeImage();

}

void ObjdetEyes::generateEyeImage()
{
    const SCRect cFaceRect = mFaceResult.rect();
    const QPoint cEyeRoiCenter
        = QPoint(cFaceRect.x()
                    + cFaceRect.width()
                        / 2 * (Objdet::EyeLeft == cmClass) ? -1 : 1,
                 cFaceRect.y() - cFaceRect.height() / 2);
    mFaceEyeRoi = SCRect(cFaceRect.size() / 2, cEyeRoiCenter)
                         .scaled(1.25).intersected(mInputFrame.rect());
    const int cThumbWidth = IfSearchWindow::faceThumbSize().width();
    mDetectScale = (mFaceEyeRoi.width() > cThumbWidth) ? 1
        : int(0.999 + qreal(cThumbWidth) / qreal(mFaceEyeRoi.width()));
    mEyeImage = mInputFrame.copy(mFaceEyeRoi)
                       .scaledToWidth(mDetectScale * mFaceEyeRoi.width());
    Objdet::inputImage(mEyeImage);
}
