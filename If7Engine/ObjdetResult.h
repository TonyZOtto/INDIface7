#pragma once

#include <QSharedDataPointer>

class ObjdetResultData;

class ObjdetResult
{
public: // our ctors

public: // const

public: // non-const

    // SharedData
public:
    ObjdetResult();
    ObjdetResult(const ObjdetResult &);
    ObjdetResult &operator=(const ObjdetResult &);
    ~ObjdetResult();
private:
    QSharedDataPointer<ObjdetResultData> data;
};
