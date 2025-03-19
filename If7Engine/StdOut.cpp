#include "StdOut.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextEdit>
#include <QWidget>

QString StdOut::mFileEndLine("\n");

StdOut::StdOut(QWidget * pCentralWidget)
    : QObject(pCentralWidget)
    , mpView(new QTextEdit(pCentralWidget))
{
    Q_ASSERT(mpView);
    setObjectName("LogOutput");
    mpView->setMinimumSize(640, 480);
    mpView->setTextCursor(mCursor);
}

StdOut::~StdOut()
{
    close();
    if (mpView) mpView->deleteLater();
}

void StdOut::write(const QString &text, const bool endl)
{
    mTextBuffer.append(text);
    write(endl);
}

void StdOut::write(QtMsgType qmt,
                  const QMessageLogContext &ctx,
                  const QString &msg)
{
    Q_ASSERT(!Q_FUNC_INFO); Q_UNUSED(qmt);
    Q_UNUSED(ctx); Q_UNUSED(msg);

}

void StdOut::write(const bool endl)
{
    if (endl)
    {
        if ( ! mTextBuffer.isEmpty())
            writeLine(mTextBuffer);
        mTextBuffer.clear();
        if (isFileOpen())
            mpTextFile->write("\n");
    }
}

void StdOut::writeLine(const QString &line)
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
        mpTextFile->write(qPrintable(line));
        write(true);
    }
}

void StdOut::writeLines(const QStringList &lines)
{
    foreach (const QString cLine, lines)
        writeLine(cLine);
}

void StdOut::writeLines(const QStringList &lines, const QString &prefix)
{
    foreach (const QString cLine, lines)
    {
        write(prefix);
        write(cLine);
        write(true);
    }
}

void StdOut::errorLine(const QString &msg, const QtMsgType qmt)
{
    write(errorPrefix(qmt));
    write(msg, true);
}

bool StdOut::isFileOpen() const
{
    return nullptr != mpTextFile;
}

bool StdOut::open(const QFileInfo &fi)
{
    close();
    QString tFileName = fi.absoluteFilePath();
    if (tFileName.isEmpty())
        tFileName = QDir::current().path() + "/log/log-@.txt";
    tFileName.replace("@", QDateTime::currentDateTime()
                               .toString("DyyyyMMdd-Thhmm"));

    QFile * pFile = new QFile(tFileName, this);
    if (pFile->open(QIODevice::WriteOnly | QIODevice::Text))
        mpTextFile = pFile;
    return isFileOpen();
}

bool StdOut::open(QtMessageHandler *pHandler)
{
    Q_ASSERT(!Q_FUNC_INFO); Q_UNUSED(pHandler);
    return false;
}


void StdOut::close()
{
    if (mpTextFile)
        mpTextFile->close();
    mpTextFile = nullptr;
}

void StdOut::writeCache()
{
    if ( ! mViewCache.isEmpty())
        writeLines(mViewCache);
    mViewCache.clear();
}

QString StdOut::errorPrefix(const QtMsgType qmt)
{
    QString result("??????: ");
    switch (qmt)
    {
    case QtInfoMsg:     result = "--INFO: ";    break;
    case QtDebugMsg:    result = ">TRACE: ";    break;
    case QtWarningMsg:  result = "*-WARN: ";    break;
    case QtCriticalMsg: result = "$ERROR: ";    break;
    case QtFatalMsg:    result = "@FATAL: ";    break;
    }
    return result;
}

