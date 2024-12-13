#pragma once

#include <QObject>

#include <QDomElement>
#include <QFileInfo>
#include <QMap>
#include <QString>

#include "Objdet.h"

class ObjdetCatalogItem;

class ObjdetCatalog : public QObject
{
    Q_OBJECT
public: // types
    typedef QPair<QString, QString> Key;

public: // ctors
    ObjdetCatalog(const QString &catXmlFileName, QObject *parent = nullptr);

public slots:

signals:

public: // const
    QFileInfo fileInfo() const;
    bool fileExists() const;
    ObjdetCatalogItem item(const Key &key) const;
    friend bool operator < (const Key &lhs, const Key &rhs);


public: // non-const
    QString readXmlFile();

public: // static
    static QString names(const Objdet::Class objcls);

private:
    QString extractClassDEs(const QDomElement &rootDE);
    QString extractDetectorItems();

private:
    QFileInfo mCatFileInfo;
    QMap<Objdet::Class, QDomElement> mClassElementMap;
    QMap<Objdet::Class, QString> mClassDefaultDetectorMap;
    QMap<Key, ObjdetCatalogItem> mKeyItemMap;
};

inline QFileInfo ObjdetCatalog::fileInfo() const { return mCatFileInfo; }
