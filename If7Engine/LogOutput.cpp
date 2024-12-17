#include "LogOutput.h"

#include <QCoreApplication>
#include <QFile>
#include <QFile>
#include <QTextEdit>
#include <QWidget>

QString LogOutput::mFileEndLine("\n");

LogOutput::LogOutput(QWidget * pCentralWidget)
    : QObject(pCentralWidget)
    , mpView(new QTextEdit(pCentralWidget))
{
    Q_ASSERT(mpView);
    setObjectName("LogOutput");
    mpView->setTextCursor(mCursor);
}

LogOutput::~LogOutput()
{
    close();
    if (mpView) mpView->deleteLater();
}

void LogOutput::write(const QString &text, const bool endl)
{
    mTextBuffer.append(text);
    write(endl);
}

void LogOutput::write(const bool endl)
{
    if (endl)
    {
        if ( ! mTextBuffer.isEmpty())
            writeLine(mTextBuffer);
        mTextBuffer.clear();
        if (isFileOpen())
            mpFile->write("\n");
    }
}

void LogOutput::writeLine(const QString &line)
{
    Q_ASSERT(mpView);
    if (mCursor.atEnd())
    {
        writeCache();
        mpView->append(line);
    }
    else
    {
        mViewCache.append(line);
    }
    if (isFileOpen())
    {
        mpFile->write(qPrintable(line));
        write(true);
    }
}

void LogOutput::writeLines(const QStringList &lines)
{
    foreach (const QString cLine, lines)
        writeLine(cLine);
}

void LogOutput::writeLines(const QStringList &lines, const QString &prefix)
{
    foreach (const QString cLine, lines)
    {
        write(prefix);
        write(cLine);
        write(true);
    }
}

bool LogOutput::isFileOpen() const
{
    return nullptr != mpFile;
}

bool LogOutput::open(const QFileInfo &fi)
{
    close();
    QFile * pFile = new QFile(fi.absoluteFilePath(), this);
    if (pFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mpFile = pFile;
    }
    return isFileOpen();
}

void LogOutput::close()
{
    if (mpFile)
        mpFile->close();
    mpFile = nullptr;
}

