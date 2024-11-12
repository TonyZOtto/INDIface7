#pragma once

#include <QSharedDataPointer>

#include <QList>

class ObjdetResultData;

class ObjdetResult
{
public: // types
    typdef QList<ObjdetResult> List;

public: // our ctors

public: // const
    qreal distance() const;

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
