#include "ObjdetCatalog.h"


ObjdetCatalog::ObjdetCatalog(const QString &catXmlFileName, QObject *parent)
    : QObject(parent)
    , mCatFileInfo(QFileInfo(catXmlFileName))
{
    VersionInfo cvv = cvVersion();
    qInfo() << "OpenCV Version:" << cvv.toString()
            << "[" << cvv.dottedString() << "]";
    qInfo() << "Catalog FileInfo" << mCatFileInfo;
}
