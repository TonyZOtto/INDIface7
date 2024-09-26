#include <FaceBase.h>

#include <QCoreApplication>
#include <QDomDocument>
#include <QString>
#include <QTime>
#include <QWriteLocker>

#include <math.h>

#include <Return.h>
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <fbPerson.h>

const QString FaceBase::templateFilePrefix = tr("FT");
const QString FaceBase::templateFileSuffix("xml");
const QString FaceBase::imageFilePrefix("FI");
const QString FaceBase::imageFileSuffix("PNG");
const QString FaceBase::personFilePrefix("PR");
const QString FaceBase::personFileSuffix("xml");

Return FaceBase::initialize(const QString & initString, int maxLoad)
{
    Return rtn;

    clear();
    if (matcher) matcher->clear();
    baseDir = QDir(qApp->applicationDirPath());
    if ( ! baseDir.exists(initString))
    {
        if ( ! baseDir.mkpath(initString))
            return Return(ReturnCreateDir, initString);
        if ( ! baseDir.cd(initString))
            return Return(ReturnChangeDir, initString);

        // TODO: Other preparation work in a new base directory

        return Return(ReturnNewBaseDir, baseDir.absolutePath());
    }

    if ( ! baseDir.cd(initString))
        return Return(ReturnChangeDir, initString);
    int nFaces = 0, nNonFace = 0, nError = 0;
    QStringList dirFilter;
    dirFilter << templateFilePrefix + "???";
    QStringList tplDirList = baseDir.entryList(dirFilter,
                                               QDir::Dirs
                                               | QDir::NoDotAndDotDot,
                                               QDir::Name);
    foreach (QString tplDirName, tplDirList)
    {
        QDir tplDir(baseDir);
        if ( ! tplDir.cd(tplDirName))
            return Return(ReturnChangeDir, tplDirName);

        QStringList nameFilter;
        nameFilter << tplDirName + "???." + templateFileSuffix;
        QStringList tplFileList = tplDir.entryList(nameFilter, QDir::Files, QDir::Name);
        foreach (QString tplFileName, tplFileList)
        {
            rtn = initFrom(tplDir.filePath(tplFileName));
            if (rtn.isWarn())
            {
                //RETURN(rtn);
                ++nError;
            }
            else if (rtn.isFalse())
            {
                ++nNonFace;
            }
            else
            {
                ++nFaces;
            }
        }

        if (maxLoad && nFaces >= maxLoad)
        {
            break;
        }
    }

    if (nError)
        return Return(ReturnInitWarning, people.size(), nFaces, nNonFace, nError);
    else
        return Return(ReturnInitialized, people.size(), nFaces, nNonFace);
}


Return FaceBase::initFrom(const QString & xmlFile)
{
    Return rtn;
    QDomDocument doc;

    EigenFaceFace face;
    rtn = face.readXmlFile(xmlFile);
    if (rtn.isWarn())
        return rtn;

    int personKey       = face.personKey();
    int faceKey		= face.faceKey();
    QString personId	= face.personId();
    QString faceId	= face.faceId();
    bool nonFace        = face.templat().isEmpty();

    if (faceKey)
    {
        peopleFaces[personKey].insertUnique(faceKey, faceId);
        mapPersonToFaces[personKey].append(faceKey);
        mapFaceToPerson.insert(faceKey, personKey);
        faceKeys.insert(faceKey);
        if (personKey && ! personId.isEmpty())
            people.insertUnique(personKey, personId);
        if (matcher && ! nonFace)
            matcher->enroll(face.templat(), faceKey, personKey);
        if (nonFace)    rtn = Return::False;
    }

    return rtn;
}

QString FaceBase::filePath(const QString & prefix, const int key,
                           const int digits, const QString & suffix) const
{
    int div = (int)pow((qreal)10.0, digits/2);
    QDir dir(baseDir);
    QString dirName = QString("%1%2").arg(prefix).arg(key/div, digits/2, 10, QChar('0'));
    dir.mkpath(dirName);
    dir.cd(dirName);
    QString fileName = tr("%1%2.%3").arg(prefix).arg(key, digits, 10, QChar('0')).arg(suffix);
    return dir.exists() ? dir.filePath(fileName) : QString();
}
