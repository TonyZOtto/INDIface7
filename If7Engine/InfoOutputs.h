/** @file InfoOutputBase.h
*	Internal declaration of InfoOutputBase class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once

#include <Info::Severity.h>
#include <NamedArray.h>

class  InfoOutputs : public NamedArray<Info::Severity, QList<InfoOutputBase *> >
{
public:
    QList<InfoOutputBase *> outputs(Info::Severity sev);
    QSet<InfoOutputBase *> unique(void);
    void clear(void);
}; // class InfoOutputs
