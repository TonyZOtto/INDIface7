#pragma once

#include <QSharedDataPointer>

#include <QDomElement>
#include <QFileInfo>
#include <QSize>
#include <QString>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include "Objdet.h"

class ObjdetCatalogItemData;

class ObjdetCatalogItem
{
public: // Our ctors
    ObjdetCatalogItem(const Objdet::Class cls);
    ObjdetCatalogItem(const QString &nm, const Objdet::Class cls);

public: // const
    bool isXmlFileValid() const;
    QSize size() const;

public: // non-const
    bool read(const QDomElement &itemDE, const QString &nm=QString());
    void name(const QString &nm);
    void oclass(const Objdet::Class cls);
    void xmlFile(const QString &nm);
    void xmlFile(const QFileInfo &fi);
    void size(const QSize &sz);
    void classfactor(const qreal f);

    // --------------------- QSharedData ----------------------
public:
    ObjdetCatalogItem();
    ObjdetCatalogItem(const ObjdetCatalogItem &);
    ObjdetCatalogItem &operator=(const ObjdetCatalogItem &);
    ~ObjdetCatalogItem();
private:
    QSharedDataPointer<ObjdetCatalogItemData> data;
};


