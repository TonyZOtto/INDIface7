/** @file Info.cpp
*	Definition of Info class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QApplication>
#include <QTimer>

#include <Info.h>



#include <Return.h>

DECLARE_NAMEDENUM_INSTANCE(Info::Severity)
DECLARE_NAMEDENUM_ID(Info::Severity, Null)
DECLARE_NAMEDENUM_ID(Info::Severity, Leave)
DECLARE_NAMEDENUM_ID(Info::Severity, Enter)
DECLARE_NAMEDENUM_ID(Info::Severity, Detail)
DECLARE_NAMEDENUM_ID(Info::Severity, Debug)
DECLARE_NAMEDENUM_ID(Info::Severity, Info)
DECLARE_NAMEDENUM_ID(Info::Severity, Progress)
DECLARE_NAMEDENUM_ID(Info::Severity, Warning)
DECLARE_NAMEDENUM_ID(Info::Severity, Error)
DECLARE_NAMEDENUM_ID(Info::Severity, Fatal)
DECLARE_NAMEDENUM_ID(Info::Severity, Unknown)

DECLARE_NAMEDARRAY_INSTANCE(Info::Severity, QList<InfoOutputBase *>)

QDateTime Info::TimeBase(QDateTime::currentDateTime());
QQueue<InfoItem> Info::queue;
QReadWriteLock Info::queueLock(QReadWriteLock::Recursive);
InfoOutputs Info::outputs;
QReadWriteLock Info::outputLock(QReadWriteLock::Recursive);
InfoThread * Info::thread = 0;
int Info::flushEvery_ = 10;
int Info::rolloverKeep_i = 0;

Info::~Info()
{
    flush();
    stop();
    // collect and delete the unique output handlers
    QSet<InfoOutputBase *> outs = outputs.unique();
    outputs.clear();
    foreach (InfoOutputBase * out, outs)
        delete out;
}

void Info::add(const Return & rtn)
{
    Info::Severity sev = Info::Severity::Info;
    switch (rtn.ddtSeverity())
    {
    case Null:			sev = Info::Severity::Null;		break;
    case Leave:			sev = Info::Severity::Leave;		break;
    case Enter:			sev = Info::Severity::Enter;		break;
    case Detail:		sev = Info::Severity::Detail;		break;
    case Debug:			sev = Info::Severity::Debug;		break;
    case Progress:		sev = Info::Severity::Progress;	break;
    case Warning:		sev = Info::Severity::Warning;	break;
    case Error:			sev = Info::Severity::Error;		break;
    case Fatal:			sev = Info::Severity::Fatal;		break;
    case Unknown:		sev = Info::Severity::Unknown;	break;
    }
    InfoItem ii(sev, QString(), 0, rtn.msg(), rtn[1], rtn[2], rtn[3], rtn[4]);
    ii.setReturnCode(rtn.returnCode());
    return add(ii);
}

bool Info::expect(const QString & strValue,
                  const QVariant & varValue,
                  const QString & strTest,
                  const QVariant & varTest,
                  const QString & file,
                  const int line)
{
    if (varValue == varTest)
        return true;

    add(InfoItem(Info::Severity::Debug, file, line, "Expected %1 (%2)==(%3) %4",
                    strValue, varValue, varTest, strTest));
    return false;
}

bool Info::expectNot(const QString & strValue,
                     const QVariant & varValue,
                     const QString & strTest,
                     const QVariant & varTest,
                     const QString & file,
                     const int line)
{
    if (varValue != varTest)
        return true;

    add(InfoItem(Info::Severity::Debug, file, line, "Expected %1 (%2)!=(%3) %4",
                    strValue, varValue, varTest, strTest));
    return false;
}

void Info::add(const InfoItem & item)
{
    {
        QWriteLocker lock(&queueLock);
        queue.enqueue(item);
    }
    if (flushEvery_ && size() >= flushEvery_)
        flush();
    if (item.severity() >= Info::Severity::Fatal)
    {
        flush();
        qFatal("Shutdown by Fatal Error");
    }
}

void Info::add(InfoOutputBase * out,
               Info::Severity minSev,
               Info::Severity maxSev)
{
    remove(out);

    QWriteLocker lock(&outputLock);
    for (int x = minSev; x <= maxSev; ++x)
        outputs[x].append(out);
}

void Info::remove(InfoOutputBase * out)
{
    QWriteLocker lock(&outputLock);
    for (int x = Info::Severity::min(); x <= Info::Severity::max(); ++x)
        outputs[x].removeAll(out);
}

void Info::flush(void)
{
    while ( ! isEmpty())
        write(take());

    foreach (InfoOutputBase * out, outputs.unique())
        out->flush();
} // flush()

void Info::write(const InfoItem & item)
{
    foreach (InfoOutputBase * out, outputs.outputs(item.severity()))
        out->write(item);
}

void Info::clear(void)
{
    foreach (InfoOutputBase * out, outputs.unique())
        out->clear();
}

void Info::start(QThread::Priority priority, int msecShort, int msecLong)
{
    if (thread)
        stop();

    thread = new InfoThread(priority, msecShort, msecLong);
    if (thread)
#ifdef INFO_NOTHREAD
        QTimer::singleShot(msecShort, thread, SLOT(pump()));
#else
        thread->start();
#endif
} // start()

void Info::stop(void)
{
    if (thread)
    {
#ifndef INFO_THREAD
        thread->quit();
        thread->wait(5000);
        if (thread->isRunning())
        {
            thread->terminate();
            thread->wait(5000);
        }
#endif
        delete thread;
        thread = 0;
    }
} // stop()

void Info::rollover(void)
{
    flush();
    foreach (InfoOutputBase * out, outputs.unique())
    {
        out->rollover();
    }
}
