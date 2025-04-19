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
//    explicit ObjdetEyes(QObject *parent = nullptr);
    ObjdetEyes(const Objdet::Class eye,
                        QObject *parent = nullptr);
    ObjdetEyes(const Objdet::Class eye,
                        const QImage &frame,
                        const DetectorResult &faceDR,
                        QObject *parent = nullptr);

public slots:
    void detectEye();

public: // const
    QImage eyeImage() const;
    QImage detectImage() const;

public: // non-const
    void generateEyeImage();

public: // pointers

private:
    QImage mInputFrame;
    DetectorResult mFaceResult;
    QImage mEyeImage;
    SCRect mFaceEyeRoi;
    int mDetectScale=0;
};

inline QImage ObjdetEyes::eyeImage() const { return mEyeImage; }

