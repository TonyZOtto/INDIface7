/** @file Info::Severity.h
*	Declaration of Info::Severity class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once

#include <NamedEnum.h>

class  Info::Severity : public NamedEnum<Info::Severity>
{
    DECLARE_NAMEDENUM_CTORS(Info::Severity)
    static const Info::Severity Null;
    static const Info::Severity Leave;
    static const Info::Severity Enter;
    static const Info::Severity Detail;
    static const Info::Severity Debug;
    static const Info::Severity Info;
    static const Info::Severity Progress;
    static const Info::Severity Warning;
    static const Info::Severity Error;
    static const Info::Severity Fatal;
    static const Info::Severity Unknown;
}; // Info::Severity
