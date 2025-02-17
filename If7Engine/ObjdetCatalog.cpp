#include "ObjdetCatalog.h"

#include "ObjdetCatalogItem.h"
#include "ObjectHelper.h"

ObjdetCatalog::ObjdetCatalog(const QString &catXmlFileName, QObject *parent)
    : QObject(parent)
    , mCatFileInfo(QFileInfo(catXmlFileName))
{
    QString result;
    if (fileExists())
        result = readXmlFile();
    if ( ! result.isEmpty())
        qCritical() << result;
}

bool ObjdetCatalog::fileExists() const
{
    return mCatFileInfo.exists();
}

ObjdetCatalogItem ObjdetCatalog::item(const ObjdetCatalogItem::Key &key) const
{
    return mKeyItemMap.value(key);
}

QString ObjdetCatalog::readXmlFile()
{
    if ( ! fileExists() )
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
    return extractClassDEs(cRoot);
}

QString ObjdetCatalog::extractClassDEs(const QDomElement &rootDE)
{
    QString result;
    QDomNode tRootNode = rootDE.firstChild();
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
                const Objdet::Class cClass = Objdet::objectClass(cClassName);
                const QDomAttr cClassDefaultAttr = tClassDE.attributeNode("Default");
                const QString cDefaultName = cClassDefaultAttr.value();
                mClassElementMap.insert(cClass, tClassDE);
                mClassDefaultDetectorMap.insert(cClass, cDefaultName);
            }
        }
        tRootNode = rootDE.nextSibling();
    }
    if (mClassElementMap.isEmpty())
        result = "No Classes in Catalog XmlFile";
    return result;
}

QString ObjdetCatalog::extractDetectorItems()
{
    QString result;
    const Objdet::ClassList cClasses = mClassElementMap.keys();
    if (cClasses.isEmpty())
        result = "No Classes in Detector XmlFile";
    else foreach (const Objdet::Class cClass, cClasses)
    {
        QDomElement tClassDE = mClassElementMap.value(cClass);
        if (Objdet::$nullClass != cClass && ! tClassDE.isNull())
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
                    ObjdetCatalogItem tItem(cDetectorName, cClass);
                    const QSize cDetectorSize(cDetectorWidth.toInt(),
                                              cDetectorHeight.toInt());
                    tItem.catalogSize(cDetectorSize);
                    mKeyItemMap.insert(tItem.key(), tItem);
                }
            }
        }
    }
    return result;
}

bool operator < (const ObjdetCatalogItem::Key &lhs,
                 const ObjdetCatalogItem::Key &rhs)
{
    if (lhs.first  < rhs.first)  return true;
    if (lhs.second < rhs.second) return true;
    return false;
}
