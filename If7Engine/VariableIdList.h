#pragma once

#include <QtCore/QList>
#include "VariableId.h"


class VariableIdList : public QList<VariableId>
{
public:
    VariableIdList(void);

    operator QStringList(void) const;

};

