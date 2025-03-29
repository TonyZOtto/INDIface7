#pragma once

#include <QObject>
#include "Objdet.h"

class ObjdetEyes : public Objdet
{
    Q_OBJECT
public:
    explicit ObjdetEyes(QObject *parent = nullptr);
};
