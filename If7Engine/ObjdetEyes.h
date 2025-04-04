#pragma once

#include <QObject>
#include "Objdet.h"

#include <QImage>
#include "DetectorResult.h"
#include "DetectorResultList.h"
#include "SCRect.h"

class ObjdetEyes : public Objdet
{
    Q_OBJECT
public: // ctors
    explicit ObjdetEyes(QObject *parent = nullptr);
    explicit ObjdetEyes(const Objdet::Class eye,
                        const QImage &frame,
                        const DetectorResult &faceDR,
                        QObject *parent = nullptr);

public slots:
    void detectEye();

public: // const

public: // non-const
    void generateDetectImage();

public: // pointers

private:
    Objdet::Class mWhichEye=Objdet::$nullClass;
    QImage mInputFrame;
    DetectorResult mFaceResult;
    QImage mDetectImage;
    SCRect mDetectRect;
    int mDetectScale=0;
    DetectorResultList mEyeResults;
};
