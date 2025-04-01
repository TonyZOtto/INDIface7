#include "ObjdetEyes.h"

#include "IfSearchWindow.h"

ObjdetEyes::ObjdetEyes(QObject *parent)
    : Objdet{Objdet::Eyes, parent}
{
    setObjectName("ObjdetEyes");
}

ObjdetEyes::ObjdetEyes(const Objdet::Class eye,
                       const QImage &frame,
                       const DetectorResult &faceDR,
                       QObject *parent)
    : Objdet{Objdet::Eyes, parent}
    , mWhichEye(eye)
    , mInputFrame(frame)
    , mFaceResult(faceDR)
{
    setObjectName("ObjdetEyes");
}

void ObjdetEyes::generateDetectImage()
{
    const SCRect cFaceRect = mFaceResult.rect();
    const QPoint cEyeRoiCenter
        = QPoint(cFaceRect.x()
                    + cFaceRect.width()
                        / 2 * (Objdet::EyeLeft == mWhichEye) ? -1 : 1,
                 cFaceRect.y() - cFaceRect.height() / 2);
    mDetectRect = SCRect(cFaceRect.size() / 2, cEyeRoiCenter)
                         .scaled(1.25).intersected(mInputFrame.rect());
    const int cThumbWidth = IfSearchWindow::faceThumbSize().width();
    mDetectScale = (mDetectRect.width() > cThumbWidth) ? 1
        : int(0.999 + qreal(cThumbWidth) / qreal(mDetectRect.width()));
    mDetectImage = mInputFrame.copy(mDetectRect)
                       .scaledToWidth(mDetectScale * mDetectRect.width());
    Objdet::inputImage(mDetectImage);
}
