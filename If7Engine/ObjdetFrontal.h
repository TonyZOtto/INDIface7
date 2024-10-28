#pragma once

#include "Objdet.h"

class ObjdetFrontal : public Objdet
{
    Q_OBJECT
public:
    explicit ObjdetFrontal(QObject *parent = nullptr);

public: // const
    QSize minSize();
    QSize maxSize();

    // ------------------ properties ---------------------
private:
    bool m_Enable;
    int m_MinAcross;
    int m_MaxAcross;
    int m_MinPixels;
    int m_MaxPixels;
    int m_MinQuality;
};
