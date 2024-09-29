#pragma once

#include "BaseIdBehavior.h"

class VariableIdBehavior : public BaseIdBehavior
{
public:
    static VariableIdBehavior * instance(void);

private:
    VariableIdBehavior(void);
    static VariableIdBehavior * singleton;
};

