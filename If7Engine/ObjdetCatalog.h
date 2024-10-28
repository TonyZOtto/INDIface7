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
    ObjdetCatalogItem item(const QString &name);


public: // non-const
    bool read();
    void item(const QString &name, const ObjdetCatalogItem &item);

private:
    QString m_DetectorsXml;
    QFileInfo mCatFileInfo;
    QMap<QString, ObjdetCatalogItem> mNameItemMap;
};
