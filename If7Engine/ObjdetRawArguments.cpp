#include "ObjdetRawArguments.h"

#include <utility>

class ObjdetRawArgumentsData : public QSharedData
{
public:
    qreal   dFactor;
    int     dNeighbors;
    int     dFlags;
    QSize   dMinSize;
    QSize   dMaxSize;
    QSize   dInputSize;
};

qreal ObjdetRawArguments::factor() const
{
    Q_ASSERT(data);
    return data->dFactor;
}

int ObjdetRawArguments::neighbors() const
{
    Q_ASSERT(data);
    return data->dNeighbors;
}

int ObjdetRawArguments::flags() const
{
    Q_ASSERT(data);
    return data->dFlags;
}

QSize ObjdetRawArguments::minSize() const
{
    Q_ASSERT(data);
    return data->dMinSize;
}

QSize ObjdetRawArguments::maxSize() const
{
    Q_ASSERT(data);
    return data->dMaxSize;
}

QSize ObjdetRawArguments::inputSize() const
{
    Q_ASSERT(data);
    return data->dInputSize;
}

cv::Size ObjdetRawArguments::cvMinSize() const
{
    Q_ASSERT(data);
    const QSize cSize = data->dMinSize;
    return cSize.isEmpty() ? cv::Size()
                :  cv::Size(cSize.width(), cSize.height());
}

cv::Size ObjdetRawArguments::cvMaxSize() const
{
    Q_ASSERT(data);
    const QSize cSize = data->dMaxSize;
    return cSize.isEmpty() ? cv::Size()
                :  cv::Size(cSize.width(), cSize.height());
}

void ObjdetRawArguments::factor(const qreal f)
{
    Q_ASSERT(data);
    data->dFactor = f;
}

void ObjdetRawArguments::neighbors(const int n)
{
    Q_ASSERT(data);
    data->dNeighbors = n;
}

void ObjdetRawArguments::flags(const int f)
{
    Q_ASSERT(data);
    data->dFlags = f;
}

void ObjdetRawArguments::minSize(const QSize sz)
{
    Q_ASSERT(data);
    data->dMinSize = sz;
}

void ObjdetRawArguments::maxSize(const QSize sz)
{
    Q_ASSERT(data);
    data->dMaxSize = sz;
}

void ObjdetRawArguments::inputSize(const QSize sz)
{
    Q_ASSERT(data);
    data->dInputSize = sz;
}

void ObjdetRawArguments::set(const Flag f)
{
    Q_ASSERT(data);
    data->dFlags |= f;
}



// -------------------- QSharedDataPointer ------------------------

ObjdetRawArguments::ObjdetRawArguments() : data(new ObjdetRawArgumentsData) {;}
ObjdetRawArguments::ObjdetRawArguments(const ObjdetRawArguments &rhs) : data{rhs.data} {;}
ObjdetRawArguments::ObjdetRawArguments(ObjdetRawArguments &&rhs) : data{std::move(rhs.data)} {;}
ObjdetRawArguments::~ObjdetRawArguments() {}

ObjdetRawArguments &ObjdetRawArguments::operator=(const ObjdetRawArguments &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

ObjdetRawArguments &ObjdetRawArguments::operator=(ObjdetRawArguments &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

