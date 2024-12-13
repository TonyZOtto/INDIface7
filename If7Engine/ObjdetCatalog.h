#pragma once

#include <QObject>

#include <QFileInfo>
#include <QMap>
#include <QString>


#include "Objdet.h"
#include "ObjdetCatalogItem.h"

class ObjdetCatalog : public QObject
{
    Q_OBJECT
public: // typedef

public: // ctors
    ObjdetCatalog(const QString &catXmlFileName, QObject *parent = nullptr);

public slots:

signals:

public: // const

    QFileInfo fileInfo() const;
    bool exists() const;
    ObjdetCatalogItem item(const QString &name);


public: // non-const
    QString readXmlFile();
    void item(const QString &name, const ObjdetCatalogItem &item);

public: // static
    static QString names(const Objdet::Class objcls);

private:
    QString extractClassDEs();
    QString extractDetectorDEs();
    QString extractItems();

private:
    QString mDetectorsXmlFileName;
    QFileInfo mCatFileInfo;
    QDomDocument mDocument;
    QDomElement mRootElement;
    QMap<QString, QDomElement> mClassNameElementMap;
    QMap<QString, ClassDetectorNames> mClassDefaultDetectorMap;
    QMap<ClassDetectorNames, QDomElement> mDetectorElementMap;
    QMap<QString, ClassDetectorNames> mClassDetectorNameMap;
    QMap<ClassDetectorNames, QSize> mClassDetectorSizeMap;
    QMap<QString, ObjdetCatalogItem> mNameItemMap;
};

inline QFileInfo ObjdetCatalog::fileInfo() const { return mCatFileInfo; }
