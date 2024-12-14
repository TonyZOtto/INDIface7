/** @file ImageCacheCleaner.cpp
*	Definition of internal ImageCacheCleaner class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <ImageCache.h>

#include <QReadWriteLock>

#include "iImageCache.h"




ImageCacheCleaner::ImageCacheCleaner(ImageCache * parent)
    : cache(parent), done(false), QThread((QObject *)parent)
{
    normalPriority = QThread::NormalPriority;
    highPriority = QThread::NormalPriority;
}

ImageCacheCleaner::~ImageCacheCleaner()
{
}

void ImageCacheCleaner::run(void)
{
    QString imageId;
    int msSleep = 1000;
    setPriority(normalPriority);

    for (;;)
    {
        if (done)
            return;

        msleep(msSleep);
        int k; // = cache->cleanNext();

        if (k > cache->maxItems)
        {
            setPriority(highPriority);
            //k = cache->cleanAll();
            setPriority(normalPriority);
        }
        msSleep = k ? 50 : 1000;
    } // forever
}


QString ImageCache::status(void)
{
    QString sts;
#ifndef TODO0002
    = tr("%1 cached, %2MB, %4 grabbed, %3 pending")
                  .arg(size())
                  .arg(curBytes / 1024 / 1024)
                  .arg(writePending.size())
                  .arg(grabPending.size());
#endif
    return sts;
} // status()

#ifndef TODO0002
int ImageCache::cleanNext(void)
{
    QString removeId;

    if (rwlUsed && ! rwlUsed->tryLockForRead(10))
        return -2;
    for (int i = 0; i < used.size(); i++)
        if ( ! isHeld(used.at(i)))
        {
            removeId = used.at(i);
            break;
        } // if (!held)
    if (rwlUsed) rwlUsed->unlock();

    if (removeId.isEmpty())
        return 0;
    else
    {
        remove(removeId);
        return used.size();
    }
} // cleanNext()
#endif

#ifndef TODO0002
int ImageCache::cleanAll(void)
{
    QStringList removeIds;

    if (rwlUsed && ! rwlUsed->tryLockForRead(10))
        return -2;

    for (int i = 0; i < used.size(); i++)
        if ( ! isHeld(used.at(i)))
            removeIds << used.at(i);
    if (rwlUsed) rwlUsed->unlock();

    if ( ! removeIds.isEmpty())
        emit remove(removeIds);
    return used.size();
} // cleanAll()
#endif
