#include "ObjdetCatalogItem.h"

#include <QFileInfo>
#include <QSize>

#include "ObjdetCatalog.h"

class ObjdetCatalogItemData : public QSharedData
{
public:
    QString                 dName;
    Objdet::Class           dClass;
    QString                 dXmlFileName;
    QFileInfo               dXmlFileInfo;
    QSize                   dSize;
    qreal                   dClassFactor;
};


ObjdetCatalogItem::ObjdetCatalogItem(const Objdet::Class cls)
    : data(new ObjdetCatalogItemData)
{
    oclass(cls);
}

ObjdetCatalogItem::ObjdetCatalogItem(const QString &nm, const Objdet::Class cls)
    : data(new ObjdetCatalogItemData)
{
    name(nm), oclass(cls);
}

QSize ObjdetCatalogItem::size() const
{
    Q_ASSERT(data);
    return data->dSize;
}

bool ObjdetCatalogItem::read(const QDomElement &itemDE, const QString &nm)
{
    bool result=true;
    if (result) result = itemDE.hasAttribute("Name");
    if ( ! result) return result;

    const QString cName = itemDE.attribute("Name");
    if ( ! nm.isEmpty() && cName != nm) result = false;
    if ( ! result) return result;

    if (result)
    {
        result &= itemDE.hasAttribute("Description");
        result &= itemDE.hasAttribute("Width");
        result &= itemDE.hasAttribute("Height");
        result &= itemDE.hasAttribute("XmlFile");
    }
    if ( ! result) return result;

    const QString cDesc = itemDE.attribute("Description");
    const QString cWidth = itemDE.attribute("Width");
    const QString cHeight = itemDE.attribute("Height");
    const QString cXmlFileName = itemDE.attribute("XmlFile");

    name(cName);
    xmlFile(cXmlFileName);
    size(QSize(cWidth.toInt(), cHeight.toInt()));
    // TODO classfactor
    result &= isXmlFileValid();
    result &= ! size().isEmpty();

    return result;
}

void ObjdetCatalogItem::name(const QString &nm)
{
    Q_ASSERT(data);
    data->dName = nm;
}

void ObjdetCatalogItem::oclass(const Objdet::Class cls)
{
    Q_ASSERT(data);
    data->dClass = cls;
}

void ObjdetCatalogItem::xmlFile(const QString &nm)
{
    Q_ASSERT(data);
    data->dXmlFileName = nm;
    data->dXmlFileInfo = QFileInfo(nm);
}

void ObjdetCatalogItem::xmlFile(const QFileInfo &fi)
{
    Q_ASSERT(data);
    data->dXmlFileInfo = fi;
    data->dXmlFileName = data->dXmlFileInfo.filePath();
}

void ObjdetCatalogItem::size(const QSize &sz)
{
    Q_ASSERT(data);
    data->dSize = sz;
}

void ObjdetCatalogItem::classfactor(const qreal f)
{
    Q_ASSERT(data);
    data->dClassFactor = f;
}

// --------------------- QSharedData ----------------------
ObjdetCatalogItem::ObjdetCatalogItem() : data(new ObjdetCatalogItemData) {;}
ObjdetCatalogItem::ObjdetCatalogItem(const ObjdetCatalogItem &rhs) : data{rhs.data} {;}
ObjdetCatalogItem::~ObjdetCatalogItem() {;}
ObjdetCatalogItem &ObjdetCatalogItem::operator=(const ObjdetCatalogItem &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

