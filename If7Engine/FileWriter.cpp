#include <FileWriter.h>
#include <FileWriteProfile.h>

#include <QTimer>

#include <Return.h>
#include <Setting.h>
#include <Settings.h>

/* USAGE: (.h)
	StatusHandler  sts;
	ImageCache cache;
	Settings * appSettings;
	FileWriter * writer;
	FileWriteProfile * fwpCapture;
	FileWriteProfile * fwpFaceCache;
	...

(.cpp)
	appSettings = new Settings(...);
	writer = new FileWriter(appSettings, QString(), this);
	writer->setImageCache(&cache);
	writer->addLogFile(&sts)
	fwpCapture = writer->newProfile("Capture", FileWriter::CaptureImage);
	fwpFaceCache = writer->newProfile("FaceCache", FileWriter::Cache | FileWriter::FaceImage);
	...
	connect(appSettings, SIGNAL(changed(QString)), writer, SLOT(settingChanged(QString)));
	writer->start();

(later)
	fwpCapture->write(imageId);		// from ImageCache
	..
	QString faceId = ...
	QImage normImage = ...
	fwpCapture->write(normImage, faceId);
*/



FileWriter::FileWriter(Settings * settings, const QString & keyFormat, QObject * parent)
    : QObject(parent), mpSettings(settings)
{
    setObjectName("FileWriter");
    mDateTime = QDateTime::currentDateTime();
    mpSettings->setValue("Output/BaseEms", mDateTime.toMSecsSinceEpoch());
    mpImageCache = 0;
    mKeyFormat = keyFormat.isEmpty() ? "Output/$Dir" : keyFormat;
    QString prefix = mKeyFormat.left(1 + mKeyFormat.lastIndexOf(QChar('/')));
    _optQuality = new Setting(settings, prefix + "Quality", -1, Settings::Volatile);
    _optFormat = new Setting(settings, prefix + "Format", "JPG", Settings::Volatile);
    _optFaceQuality = new Setting(settings, prefix + "FaceQuality", -1, Settings::Volatile);
    _optFaceFormat = new Setting(settings, prefix + "FaceFormat", "PNG", Settings::Volatile);
    _optBaseDir = new Setting(settings, prefix + "BaseDir", "../Output", Settings::Volatile);
    _optMaxCache = new Setting(settings, prefix + "MaxCache", 64, Settings::Volatile);
    Return::add(ReturnNoCache, "No Image Cache for %1", Warning);
    Return::add(ReturnNullImage, "Null Image for %2: %1", Warning);
}

FileWriter::~FileWriter()
{
    pumpAll();
}

FileWriteProfile * FileWriter::newProfile(const QString & name, QString key)
{
    return newProfile(name, $null, key);
}

FileWriteProfile * FileWriter::newProfile(const QString & name, Flags f, QString key)
{

    bool isCache = mCacheDirNameList.contains(name, Qt::CaseInsensitive);
    if (isCache) f |= Cache;
    FileWriteProfile * profile = new FileWriteProfile(name, f, this);
    if ( ! profile)                                                 return 0;

    if (key.isEmpty())
    {
        key = mKeyFormat;
        key.replace("$", name);
    }

    mKeyProfileMap.insert(key, profile);
    profile->_opt = new Setting(mpSettings, key, QString(), Settings::Volatile);
    if (XmlText & profile->_flags) profile->_format = "xml";

    return profile;
} // newProfile()

FileWriteProfile * FileWriter::profile(const QString & name) const
{
    return mKeyProfileMap.contains(name)
    ? mKeyProfileMap.value(name) : 0;
}

void FileWriter::setCacheDirs(const QString & cacheDirs)
{
    mCacheDirNameList = cacheDirs.simplified().split(' ');
}


void FileWriter::start(int longMsec, int shortMsec, int cacheMsec)
{
    mLongMsec = longMsec, mShortMsec = shortMsec, mCacheMsec = cacheMsec;

    enqueue(0, _optQuality->keyName());
    enqueue(0, _optFormat->keyName());
    enqueue(0, _optFaceQuality->keyName());
    enqueue(0, _optFaceFormat->keyName());
    enqueue(0, _optMaxCache->keyName());
    enqueue(0, _optQuality->keyName());
    foreach (FileWriteProfile * prof, mKeyProfileMap)
        enqueue(prof, QString());

    QTimer::singleShot(mShortMsec, this, SLOT(pulse()));
    QTimer::singleShot(mCacheMsec, this, SLOT(cacheClean()));
} // start()

QString FileWriter::timeStampString(void)
{
    if (mTimeStampString.isEmpty())
        mTimeStampString = mDateTime.toString("DyyyyMMdd-Thhmm");
    return mTimeStampString;
} // timeStampString()

void FileWriter::pulse(void)
{
    // TODO: investigate worker QThread to service queue
    //    FUNCTION();
    QTimer::singleShot(pumpFirst() ? mShortMsec : mLongMsec, this, SLOT(pulse()));
} // pulse()


void FileWriter::settingChanged(QString key)
{
    if (_optQuality->keyName() == key)
        enqueue(0, key);
    else if (_optFormat->keyName() == key)
        enqueue(0, key);
    else if (_optFaceQuality->keyName() == key)
        enqueue(0, key);
    else if (_optFaceFormat->keyName() == key)
        enqueue(0, key);
    else if (_optMaxCache->keyName() == key)
        enqueue(0, key);
    else if (_optBaseDir->keyName() == key)
        enqueue(0, key);
    else if (mKeyProfileMap.contains(key))
        enqueue(mKeyProfileMap.value(key), QString());
    pumpAll();
} // settingChanged()

void FileWriter::setImageCache(ImageCache * cache)
{
    mpImageCache = cache;
} // setImageCache()

bool FileWriter::isQueueValid(void)
{
    //    FUNCTION();
    QReadLocker lock(&mQueueLock);
    int nProfile	= mProfileQueue.size();
    int nName		= mNameQueue.size();
    int nData		= mBytesQueue.size();
    bool valid		= (nProfile == nName && nProfile == nData);
    if ( ! valid)
        emit error(tr("FileWriter Queue is not valid %1 %2 %3").arg(nProfile).arg(nName).arg(nData));
    //FNRETURN(valid);
    return valid;
} // isQueueValid()

bool FileWriter::isQueueEmpty(void)
{
    //    FUNCTION();
    QReadLocker lock(&mQueueLock);
    int nProfile	= mProfileQueue.size();
    int nName		= mNameQueue.size();
    int nData		= mBytesQueue.size();
    bool empty		= (0 == nProfile && 0 == nName && 0 == nData);
    //FNRETURN(empty);
    return empty;
} // isQueueEmpty()

void FileWriter::clearQueue(void)
{
    //    FUNCTION();
    QWriteLocker lock(&mQueueLock);
    mProfileQueue.clear();
    mNameQueue.clear();
    mBytesQueue.clear();
} // clearQueue()

void FileWriter::enqueue(FileWriteProfile * profile,
                         const QString & name,
                         const QByteArray & ba)
{
    //    FUNCTION();
    if ( ! isQueueValid())
        clearQueue();
    QWriteLocker lock(&mQueueLock);
    mProfileQueue.enqueue(profile);
    mNameQueue.enqueue(name);
    mBytesQueue.enqueue(ba);
    if (profile && ! name.isEmpty() && ! ba.isEmpty())
        profile->_filesPending.append(name);
} // enqueue()

bool FileWriter::dequeue(FileWriteProfile ** profile,
                         QString * name,
                         QByteArray * ba)
{
    //    FUNCTION();
    if (isQueueEmpty())
    {
        //FNRETURN("empty");
        return false;
    }
    if ( ! isQueueValid())
    {
        clearQueue();
        //FNRETURN("invalid");
        return false;
    }
    QWriteLocker lock(&mQueueLock);
    if (profile)
        *profile = mProfileQueue.dequeue();
    if (name)
        *name = mNameQueue.dequeue();
    if (ba)
        *ba = mBytesQueue.dequeue();
    //FNRETURN("success");
    return true;
} // dequeue()

void FileWriter::pumpAll(void)
{
    //    FUNCTION();
    while (pumpFirst())
        ;
} // pumpAll()

int FileWriter::pumpFirst(void)
{
    //    FUNCTION();
    FileWriteProfile * profile;
    QString name;
    QByteArray ba;
    if ( ! dequeue(&profile, &name, &ba))
        return 0;

    if (profile)
    {
        if (ba.isEmpty())
        {
            // new set of directories
            setupDirs(profile);
        }
        else
        {
            // write file to directories
            queuedWrite(profile, name, ba);
        }
    }
    else
    {
        if (ba.isEmpty())
        {
            // parameter change
            queuedChange(name);
        }
        else
        {
        }
    }

    if ( ! isQueueValid())
        clearQueue();

    return mProfileQueue.size();
} // pumpFirst()


void FileWriter::queuedChange(const QString & key)
{
    //    FUNCTION();
    if (0 == key.compare(_optQuality->keyName(), Qt::CaseInsensitive))
    {
        // new quality
        int q = _optQuality->toInt();
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            if ( ! prof->_flags.testFlag(FaceImage))
                prof->_quality = q;
    }
    else if (0 == key.compare(_optFormat->keyName(), Qt::CaseInsensitive))
    {
        // new format
        QString fmt = _optFormat->toString();
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            if ( ! prof->_flags.testFlag(FaceImage) && ! prof->_flags.testFlag(XmlText))
                prof->_format = fmt;
    }
    else if (0 == key.compare(_optFaceQuality->keyName(), Qt::CaseInsensitive))
    {
        // new face quality
        int q = _optFaceQuality->toInt();
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            if (FaceImage & prof->_flags)
                prof->_quality = q;
    }
    else if (0 == key.compare(_optFaceFormat->keyName(), Qt::CaseInsensitive))
    {
        // new face format
        QString fmt = _optFaceFormat->toString();
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            if (FaceImage & prof->_flags)
                prof->_format = fmt;
    }
    else if (0 == key.compare(_optBaseDir->keyName(), Qt::CaseInsensitive))
    {
        // new base dir
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            setupDirs(prof);
    }
    else if (0 == key.compare(_optMaxCache->keyName(), Qt::CaseInsensitive))
    {
        // new max cache
        int m = _optMaxCache->toInt();
        foreach(FileWriteProfile * prof, mKeyProfileMap)
            if (Cache & prof->_flags)
                prof->_maxCache = m;
    }
    else
        emit error(tr("Unhandled key: %1").arg(key));

} // queuedChange()

QDir FileWriter::baseDir(void)
{
    QDir dir(QDir::current());

    QString baseName = _optBaseDir->toString();
    baseName.replace('@', timeStampString());
    if ( ! baseName.isEmpty())
    {
        if ( ! dir.mkpath(baseName))
            emit error("Unable to make path: " + baseName);
        if ( ! dir.cd(baseName))
            emit error("Unable to change to path: " + baseName);
    }

    return dir;
} // baseDir()

void FileWriter::setupDirs(FileWriteProfile * profile)
{
    //    FUNCTION();
    Q_ASSERT(profile);
    profile->_dirs.clear();
    QString delimitedNames = profile->_opt->toString();
    if (delimitedNames.isEmpty())
    {
        emit directorySet(profile->name(), QString());
        return;
    }

    foreach (QString dirName, delimitedNames.split(';'))
    {
        dirName = dirName.simplified();
        if (dirName.isEmpty())
            continue;

        QDir dir = baseDir();

        dirName.replace('@', timeStampString());
        if ( ! dirName.isEmpty())
        {
            if ( ! dir.mkpath(dirName))
                emit error("Unable to make path: " + dirName);
            if ( ! dir.cd(dirName))
                emit error("Unable to change to path: " + dirName);
        }

        profile->_dirs.append(dir);

        if (Cache & profile->_flags)
            foreach (QString fileName, dir.entryList(QDir::Files, QDir::Unsorted))
                if ( ! dir.remove(fileName))
                    emit error("Error removing " + dir.absoluteFilePath(fileName));

        emit directorySet(profile->name(), dir.absolutePath());
    }

    mCacheDirQueue.clear();
    foreach (FileWriteProfile * fwp, mKeyProfileMap.values())
        foreach(QDir dir, fwp->dirs())
            if (fwp->_flags & FileWriter::Cache)
                mCacheDirQueue.enqueue(dir);
} // setupDirs()

void FileWriter::queuedWrite(FileWriteProfile * profile, const QString & baseName, const QByteArray & ba)
{
    //    FUNCTION();
    Q_ASSERT(profile);
    bool tmp = false; // TODO-Test: TempAndRename & profile->_flags;
    foreach (QDir dir, profile->_dirs)
    {
        QString fileName;
        if (CaptureImage & profile->_flags)
            fileName = dir.absoluteFilePath(baseName);
        else if (Copy & profile->_flags)
            fileName = dir.absoluteFilePath(baseName + "." + QFileInfo((QString)ba).suffix());
        else
            fileName = dir.absoluteFilePath(baseName + "." + profile->_format);

        QFileInfo fi(fileName);
        QString path = fi.path();
        if ( ! dir.exists(path))
        {
            if ( ! dir.mkpath(path))
                emit error("Unable to make path: " + path);
            if ( ! dir.cd(path))
                emit error("Unable to change to path: " + path);
        }

        if (Copy & profile->_flags)
        {
            QFile file((QString)ba);
            if (tmp)
            {
                if ( ! file.copy(fileName + ".TMP"))
                    emit error(Return::qfileError(file).toString());
                else if ( ! QFile::rename(fileName + ".TMP", fileName))
                    emit error("Error renaming to " + fileName);
            }
            else if ( ! file.copy(fileName))
                emit error(Return::qfileError(file).toString());
        }
        else
        {
            QFile file(fileName + (tmp ? ".TMP" : ""));
            if ( ! file.open(QIODevice::WriteOnly))
                emit error(Return::qfileError(file).toString());
            else if (ba.size() != file.write(ba))
                emit error(Return::qfileError(file).toString());
            else if (tmp)
            {
                if ( ! QFile::rename(fileName + ".TMP", fileName))
                    emit error("Error renaming to " + fileName);
            }
        }

        profile->_filesPending.removeAll(baseName);
        emit fileWritten(fileName);
    }
} // queuedWrite()

void FileWriter::cacheClean(void)
{
    int maxCache = _optMaxCache->toInt();
    if (maxCache && ! mCacheDirQueue.isEmpty())
    {
        QDir dir(mCacheDirQueue.dequeue());
        QStringList fileNames = dir.entryList(QDir::Files, QDir::Time);
        if (fileNames.size() > ((5 * maxCache) / 4))
        {
            while (fileNames.size() > maxCache)
            {
                QString fileName = fileNames.takeLast();
                if ( ! dir.remove(fileName))
                    emit error("Error removing " + dir.absoluteFilePath(fileName));
            }
        }
        mCacheDirQueue.enqueue(dir);
    }

    QTimer::singleShot(mCacheMsec, this, SLOT(cacheClean()));
} // cacheClean()

void FileWriter::dump(void) const
{
    foreach (QString name, mKeyProfileMap.keys())
    {
        FileWriteProfile * fwp = profile(name);
        if (fwp)
        {
            if (fwp->isActive())
                fwp->dump();
        }
        else
        {
            qWarning() << Q_FUNC_INFO << __LINE__ << "null FileWriteProfile" << name;
        }
    }
} // dump()

