/** @file FileWriter.h
*	Declaration of FileWriter class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

class QDir;
class QFile;
#include <QByteArray>
#include <QDateTime>
#include <QFlags>
#include <QMap>
#include <QQueue>
#include <QString>
#include <QStringList>
#include <QReadWriteLock>

#include <DDTcore.h>

class Setting;
class Settings;
class StatusHandler;
class ImageCache;
class FileWriteProfile;

class FileWriter : public QObject
{
    Q_OBJECT

    friend class FileWriteProfile;
public:
    enum Flag
    {
        $null       =   0,
        Cache		=  1,	// Manage cache size
        FaceImage	=  2,	// Use FaceQuality/Format
        XmlText		=  4,	// xml file extension
        CaptureImage	=  8,	// Copy from cache BA data
        Copy		= 16,	// Copy existing file
        TempAndRename	= 32,
        OtherImage      = 64,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    enum
    {
        ReturnNoCache = 0x88010001,
        ReturnNullImage,
    };

public:
    FileWriter(Settings * settings, const QString & keyFormat=QString(), QObject * parent=0);
    ~FileWriter(void);
    void setCacheDirs(const QString & cacheDirs);
    FileWriteProfile * newProfile(const QString & name, Flags f=$null, QString key=QString());
    FileWriteProfile * newProfile(const QString & name, QString key);
    FileWriteProfile * profile(const QString & name) const;
    void setImageCache(ImageCache * cache);
    bool addLogFile(StatusHandler * sts);
    void start(int longMsec=500, int shortMsec=50, int cacheMsec=60000);
    bool isQueueValid(void);
    bool isQueueEmpty(void);
    int pumpFirst(void);
    void pumpAll(void);
    QString timeStampString(void);
    QDir baseDir(void);
    int size(void) const { return mProfileQueue.size(); }
    void dump(void) const;

public slots:
    void settingChanged(QString key);

signals:
    void directorySet(QString profileName, QString absoluteDirName);
    void fileWritten(QString absoluteFilePath);
    void error(QString message);

private:	// for friends
    void enqueue(FileWriteProfile * profile,
                 const QString & name,
                 const QByteArray & ba=QByteArray());

private:
    void clearQueue(void);
    bool dequeue(FileWriteProfile ** profile,
                 QString * name,
                 QByteArray * ba);
    void queuedChange(const QString & key);
    void setupDirs(FileWriteProfile * profile);
    void queuedWrite(FileWriteProfile * profile,
                     const QString & name,
                     const QByteArray & ba);

private slots:
    void pulse(void);
    void cacheClean(void);

private:
    QDateTime mDateTime;
    QString mTimeStampString;
    Settings * mpSettings=nullptr;
    QString mKeyFormat;
    ImageCache * mpImageCache=nullptr;
    Setting * _optQuality=nullptr;
    Setting * _optFormat=nullptr;
    Setting * _optFaceQuality=nullptr;
    Setting * _optFaceFormat=nullptr;
    Setting * _optBaseDir=nullptr;
    Setting * _optMaxCache=nullptr;
    int	mLongMsec;
    int	mShortMsec;
    int	mCacheMsec;
    QStringList mCacheDirNameList;
    QMap<QString, FileWriteProfile *> mKeyProfileMap;
    // pending queue
    QReadWriteLock mQueueLock;
    QQueue<FileWriteProfile *> mProfileQueue;
    QQueue<QString> mNameQueue;
    QQueue<QByteArray> mBytesQueue;
    QQueue<QDir> mCacheDirQueue;

}; // class FileWriter

Q_DECLARE_OPERATORS_FOR_FLAGS(FileWriter::Flags);
