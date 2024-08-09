#pragma once

#include <QDomElement>
#include <QLine>
#include <QPoint>
#include <QSize>

#include "QQRect.h"

class DomValuesElement
{
public: // ctors
    DomValuesElement(QDomElement * pDE=nullptr);
    DomValuesElement(const QString &aElementTag,
               QDomElement * pDE=nullptr);

public: // const

public: // non-const
    void setTag(const QString &aTag);
    void append(const int i, const QString aName);
    void append(const QLine ln, const QString aName);
    void append(const QPoint pt, const QString aName);
    void append(const QQRect rc, const QString aName);
    void append(const QSize sz, const QString aName);
    void append(const QString &s, const QString aName);

private:
    QDomElement * pParentDE=nullptr;
    const QString cmTag;
};
