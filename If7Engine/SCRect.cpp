#include "SCRect.h"

SCRect::SCRect() : mSize(0,0), mCenter(0,0) {;}
SCRect::SCRect(const QSize sz, const QPoint cpt) : mSize(sz), mCenter(cpt)  {;}
SCRect::SCRect(const QRect qrc) : mSize(qrc.size()), mCenter(qrc.center())  {;}

int SCRect::top() const
{
    return center().y() + size().height() / 2;
}

int SCRect::left() const
{
    return center().x() + size().width() / 2;
}

bool SCRect::contains(const QPoint pt) const
{
    return toQRect().contains(pt);
}

SCRect SCRect::scaled(const qreal f) const
{
    return SCRect((sizeF() * f).toSize(), center());
}

SCRect SCRect::trimmed(const int i) const
{
    SCRect result(*this);
    const int cMask = i - 1;
    result.width(width() & cMask),
        result.height(height() & ~ cMask);
    return result;
}

SCRect SCRect::intersected(const SCRect other) const
{
    return SCRect(toQRect().intersected(other.toQRect()));
}

