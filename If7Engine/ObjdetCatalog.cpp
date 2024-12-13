#include "ObjdetCatalog.h"

#include "ObjectHelper.h"

ObjdetCatalog::ObjdetCatalog(const QString &catXmlFileName, QObject *parent)
    : QObject(parent)
    , mCatFileInfo(QFileInfo(catXmlFileName))
{
}

bool ObjdetCatalog::exists() const
{
    return mCatFileInfo.exists();
}

QString ObjdetCatalog::readXmlFile()
{
    if ( ! exists() )
        return "Catalog XmlFile doesn't exist";                     /*=====*/
    QFile * pFile = new QFile(fileInfo().absoluteFilePath(), this);
    if ( ! pFile->open(QIODevice::ReadOnly | QIODevice::Text))
        return "Unable to open Catalog XmlFile";                    /*=====*/
    const QByteArray cBytes = pFile->readAll();
    pFile->close();
    delete pFile; pFile = nullptr;
    QDomDocument tDoc("Catalog");
    if ( ! tDoc.setContent(cBytes))
        return "Invalid Catalog XmlFile document";                  /*=====*/
    const QDomElement cRoot = tDoc.documentElement();
    if (cRoot.isNull())
        return "Catalog Xml is Null";                               /*=====*/
    mDocument = tDoc, mRootElement = cRoot;

    QString result;
    if (result.isEmpty())   result = extractDetectorDEs();
    if (result.isEmpty())   result = extractItems();
    return result;
}

QString ObjdetCatalog::extractClassDEs()
{
    QString result;
    QDomElement tRootDE = mRootElement;
    QDomNode tRootNode = tRootDE.firstChild();
    while ( ! tRootNode.isNull())
    {
        QDomNode tClassNode = tRootNode.namedItem("DetectorClass");
        if (tClassNode.isElement())
        {
            QDomElement tClassDE = tClassNode.toElement();
            if ( ! tClassDE.isNull())
            {
                const QDomAttr cClassNameAttr = tClassDE.attributeNode("Name");
                const QString cClassName = cClassNameAttr.value();
                mClassNameElementMap.insert(cClassName, tClassDE);
                const QDomAttr cClassDefaultAttr = tClassDE.attributeNode("Default");
                const QString cDefaultName = cClassDefaultAttr.value();
                const ClassDetectorNames cCDN(cClassName, cDefaultName);
                mClassDefaultDetectorMap.insert(cClassName, cCDN);
            }
        }
        tRootNode = tRootDE.nextSibling();
    }
    if (mClassNameElementMap.isEmpty())
        result = "No Classes in Catalog XmlFile";
    return result;
}

QString ObjdetCatalog::extractDetectorDEs()
{
    QString result;
    const QStringList cClassNames = mClassNameElementMap.keys();
    if (cClassNames.isEmpty())
        result = "No ClassNames in Detector XmlFile";
    else foreach (const QString cClassName, cClassNames)
    {
        QDomElement tClassDE = mClassNameElementMap.value(cClassName);
        if ( ! tClassDE.isNull())
        {
            QDomNode tChildNode = tClassDE.firstChild();
            if ( ! tChildNode.isNull())
            {
                QDomNode tDetectorNode = tChildNode.namedItem("Detector");
                if (tDetectorNode.isElement())
                {
                    QDomElement tDetectorDE = tDetectorNode.toElement();
                    const QString cDetectorName = tDetectorDE.attribute("Name");
                    const QString cDetectorWidth = tDetectorDE.attribute("Width");
                    const QString cDetectorHeight = tDetectorDE.attribute("Height");
                    const ClassDetectorNames cNames(cClassName, cDetectorName);
                    const QSize cDetectorSize(cDetectorWidth.toInt(),
                                              cDetectorHeight.toInt());
                    mClassDetectorNameMap.insert(cClassName, cNames);
                    mDetectorElementMap.insert(cNames, tDetectorDE);
                    mClassDetectorSizeMap.insert(cNames, cDetectorSize);
                }
            }
        }
    }
    return result;
}

