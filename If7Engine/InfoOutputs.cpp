/** @file InfoOutputs.cpp
*	Definition of InfoOutputs class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "InfoOutputs.h"


#include <Info.h>
		
QList<InfoOutputBase *> InfoOutputs::outputs(Info::Severity sev)
{
	QReadLocker lock(Info::outLock());
	return at(sev);
}

QSet<InfoOutputBase *> InfoOutputs::unique(void)
{
	QSet<InfoOutputBase *> outs;
	QReadLocker lock(Info::outLock());
	for (int x = Info::Severity::min(); x <= Info::Severity::max(); ++x)
		outs |= at(x).toSet();
	return outs;
}

void InfoOutputs::clear(void)
{
	QWriteLocker lock(Info::outLock());
	for (int x = Info::Severity::min(); x <= Info::Severity::max(); ++x)
		value(x).clear();
}
