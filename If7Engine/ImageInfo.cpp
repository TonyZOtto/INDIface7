/** @file ImageInfo.cpp
*	Definition of ImageInfo class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QDomDocument>
#include <QImage>
#include <QtDebug>

#include <ImageInfo.h>


#ifndef TODO0002

ImageInfo::ImageInfo(void)
{
    mAttributeSet.clear();
    mFInfoList.clear();
} // default c'tor
		
ImageInfo::ImageInfo(const QImage & image)
{
	QString errorMsg;
	int errorLine, errorCol;

	QString xmlInfo = image.text("INDIinfo");
	if ( ! xmlInfo.isEmpty())
	{
		QDomDocument doc;
		if (doc.setContent(xmlInfo, &errorMsg, &errorLine, &errorCol))
		{
			read(doc.documentElement());
		}
	}
} // ImageInfo c'tor

ImageInfo::~ImageInfo()
{
} // d'tor


ImageInfo & ImageInfo::operator=(const ImageInfo & that)
{
//	aSet.clear();
    this->mAttributeSet  = that.mAttributeSet;
//	fSets.clear();
    this->mFInfoList = that.mFInfoList;
	return *this;
}

#ifndef TODO0002
bool ImageInfo::read(const QDomElement & de)
{
	QDomNodeList nodesFaces = de.elementsByTagName("Faces");
	for (int kNode = 0; kNode < nodesFaces.size(); ++kNode)
	{
		QDomNode nodeFace = nodesFaces.at(kNode);
		QDomElement elementFeatureSet = nodeFace.firstChildElement("FeatureSet");
		while ( ! elementFeatureSet.isNull())
		{
            FeatureInfo fInfo(elementFeatureSet);
            mFInfoList << fInfo;
			elementFeatureSet = elementFeatureSet.nextSiblingElement("FeatureSet");
		}
	}
	return true;
} // read(QDomElement)
#endif

void ImageInfo::addFace(const FeatureInfo &fInfo)
{
    mFInfoList.append(fInfo);
}

FeatureInfo ImageInfo::face(int x) const
{
    return (x >=0 && x < mFInfoList.size())
               ? mFInfoList.at(x)
               : FeatureInfo();
}

#ifndef TODO0002
QList<QPoint> ImageInfo::eyes(void) const
{
	QList<QPoint> list;
    foreach(FeatureSet fSet, mFInfoList)
	{
		QPoint l = fSet.get(Feature::LeftEye).toPoint();
		QPoint r = fSet.get(Feature::RightEye).toPoint();
		if ( ! l.isNull() && ! r.isNull())
			list << l << r;
	}
	return list;
}

QList<QRect> ImageInfo::heads(void) const
{
	QList<QRect> list;
    foreach(FeatureSet fSet, mFInfoList)
	{
		QRect rect = fSet.get(Feature::HeadBox).toRect();
		if (rect.isValid())
			list << rect;
	}
	return list;
}

QList<QRect> ImageInfo::heads(QString usage) const
{
	QList<QRect> list;
    foreach(FeatureSet fSet, mFInfoList)
	{
		QRect rect = fSet.get(Feature::HeadBox).toRect();
		QString u = fSet.get(Feature::Usage).toString();
		if (rect.isValid() && u == usage)
			list << rect;
	}
	return list;
}
#endif

bool ImageInfo::isEmpty(void)
{
    return mAttributeSet.isEmpty() && mFInfoList.isEmpty();
//	return fSets.isEmpty();
}
		
bool ImageInfo::writeDomElement(QDomElement * pDE) const
{
    QDomElement faces = pDE->ownerDocument()
                            .createElement("Faces");
    pDE->appendChild(faces);
    foreach(const FeatureInfo cFInfo, mFInfoList)
	{
        QDomElement tFaceDE = pDE->ownerDocument()
                        .createElement("FeatureInfo");
//        cFInfo.fillDomElement(&tFaceDE);
        faces.appendChild(tFaceDE);
	}
	return true;
} // writeDomElement()
		
bool ImageInfo::setImageText(QImage * image, QString tag)
{
	if (isEmpty() || ! image || image->isNull())
		return false;

	QDomDocument doc(tag);
	QDomElement root = doc.createElement(tag);
	doc.appendChild(root);
	writeDomElement(&root);
	QString xml = doc.toString();
	image->setText("INDIinfo", xml);
	return true;
} // setImageText()
#endif

