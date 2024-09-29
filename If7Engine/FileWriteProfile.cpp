#include <FileWriteProfile.h>
#include <FileWriter.h>

#include <QBuffer>
#include <QDomDocument>

#include <ImageCache.h>
#include <Return.h>
#include <Setting.h>
#include <Settings.h>

FileWriteProfile::FileWriteProfile(const QString & name,
                                   FileWriter::Flags f,
                                   FileWriter * parent)
    : _name(name)
    , _flags(f)
    , _opt(0)
    , _quality(0)
    , _maxCache(0)
    , QObject(parent)
{
    setObjectName("FileWriteProfile::"+name);
}

FileWriteProfile::~FileWriteProfile()
{
}

FileWriter * FileWriteProfile::writer(void) const	
{ 
    return qobject_cast<FileWriter *>(parent());
}

QString FileWriteProfile::filePath(const QString & baseName)
{
    if (_dirs.isEmpty())
        return QString();
    else
        return _dirs.at(0).absoluteFilePath(baseName + "." + _format);
} // filePath()

QString FileWriteProfile::keyName(void) const
{
    return _opt->keyName();
}

Return FileWriteProfile::write(const QString & imageId)
{
    Q_ASSERT(writer());
    if ( ! writer()->mpImageCache)
    {
        return Return(FileWriter::ReturnNoCache, imageId);
    }

    QString baseName = imageId + "." + writer()->mpImageCache->format(imageId);
    QByteArray ba = writer()->mpImageCache->getData(imageId);
    writer()->enqueue(this, baseName, ba);

    return Return();
} // write(imageId)

Return FileWriteProfile::write(QImage image,
                               const QString & baseName,
                               const QDomDocument & doc)
{
    Q_ASSERT(writer());
    if (image.isNull())
        return Return(FileWriter::ReturnNullImage, baseName, name());

    if ( ! doc.isNull())
        image.setText(doc.doctype().name(), doc.toString(4));

    return write(image, baseName);
} // write(image+doc)

Return FileWriteProfile::write(const QImage & image, const QString & baseName)
{
    Q_ASSERT(writer());
    if (image.isNull())
        return Return(FileWriter::ReturnNullImage, baseName, name());

    QByteArray ba;
    QBuffer buffer(&ba);
    if ( ! image.save(&buffer, qPrintable(_format), _quality))
        return Return(FileWriter::ReturnNullImage, baseName, name());

    writer()->enqueue(this, baseName, ba);
    return Return();
} // write(QImage)

Return FileWriteProfile::write(const QByteArray & ba, const QString & baseName)
{
    Q_ASSERT(writer());
    writer()->enqueue(this, baseName, ba);
    return Return();
} // write(QByteArray)

Return FileWriteProfile::write(const QFile & file, QString baseName)
{
    Q_ASSERT(writer());
    QByteArray ba(qPrintable(file.fileName()));
    if (baseName.isEmpty())
        baseName = QFileInfo(file.fileName()).baseName();
    writer()->enqueue(this, baseName, ba);
    return Return();
} // write(QFile)

Return FileWriteProfile::write(const QDomDocument & doc, const QString & baseName)
{
    Q_ASSERT(writer());
    QByteArray ba = doc.toByteArray(2);
    writer()->enqueue(this, baseName, ba);
    return Return();
} // write(QDomDocument)

void FileWriteProfile::dump(void) const
{
    QString flags(" ");
    if (_flags & FileWriter::Cache)             flags += "Cache ";
    if (_flags & FileWriter::FaceImage)         flags += "FaceImage ";
    if (_flags & FileWriter::XmlText)           flags += "XmlText ";
    if (_flags & FileWriter::CaptureImage)      flags += "CaptureImage ";
    if (_flags & FileWriter::Copy)              flags += "Copy ";
    if (_flags & FileWriter::TempAndRename)     flags += "TempAndRename ";
    if (_flags & FileWriter::OtherImage)        flags += "OtherImage ";
    qInfo() << Q_FUNC_INFO << flags;
} // dump()
