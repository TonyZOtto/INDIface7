/** @file ImageInfo.h
*	Public declaration of ImageInfo class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once

#include <qglobal.h>
#ifdef DDTIMG_LIB
# define DDTIMG_EXPORT Q_DECL_EXPORT
#else
# define DDTIMG_EXPORT Q_DECL_IMPORT
#endif

class QDomDocument;
#include <QList>
#include <QObject>

#include <AttributeSet.h>
#include <FeatureInfo.h>

class DDTIMG_EXPORT ImageInfo
{
#ifndef TODO0002
public:
    ImageInfo(void);
    ImageInfo(const QImage & image);
    ~ImageInfo();
    ImageInfo & operator=(const ImageInfo & that);
    void addFace(const FeatureInfo & fInfo);
    FeatureInfo face(int x) const;
    bool writeDomElement(QDomElement * pDE) const;
    bool read(const QDomElement & de);
    bool setImageText(QImage * image, QString tag="INDIinfo");
    bool isEmpty(void);
    //		void clear(void) { aSet.clear(), mFInfoList.clear(); }
    void clear(void) { mFInfoList.clear(); }
    void clearFaces(void) { mFInfoList.clear(); }
    QList<QPoint> eyes(void) const;
    QList<QRect> heads(void) const;
    QList<QRect> heads(QString usage) const;

private:
    AttributeSet        mAttributeSet;
    QList<FeatureInfo> mFInfoList;
#endif
}; // class ImageCache
