#pragma once

#include <QList>

#include "AbstractIdString.h"

class VariableId : public AbstractIdString
{
public:
    VariableId(const QString & inString=QString());
    VariableId(const char * chars);
};


