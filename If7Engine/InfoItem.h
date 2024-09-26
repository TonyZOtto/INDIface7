/** @file InfoItem.h
*	Internal declaration of InfoItem class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once

#include <Info.h>

#include <QString>
#include <QTime>
#include <QVariant>
class QTextStream;
class QThread;

#include "Info.h"

class  InfoItem
{
public:
    InfoItem(void);
    InfoItem(Info::Severity Sev, const QString & File, const int Line, const QString & Msg, QVariant V1=QVariant(),
             QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
    InfoItem(Info::Severity Sev, const QString & Msg, QVariant V1=QVariant(),
             QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
    ~InfoItem() {;}
    Info::Severity severity(void) const { return sev; }
    void clear(void) { *this = InfoItem(); }
    void setReturnCode(int rtn) { rc = rtn; }
    int returnCode(void) const { return rc; }
    int msecSince(void) const;
    QTime time(void) const { return ts; }
    QString stringOnly(void) const;
    QString toString(bool withTime=true, bool richText=true) const;

private:
    QThread *	thread;
    QTime		ts;
    int			rc;
    Info::Severity sev;
    QString		file;
    int			line;
    QString		msg;
    QVariant	v1;
    QVariant	v2;
    QVariant	v3;
    QVariant	v4;
}; // class InfoItem
