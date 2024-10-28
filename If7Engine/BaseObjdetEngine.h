#pragma once

#include <QObject>

#include "Objdet.h"

class BaseObjdetEngine : public QObject
{
    Q_OBJECT
public: // ctors
    explicit BaseObjdetEngine(QObject *parent = nullptr);
protected:
    BaseObjdetEngine(const Objdet::Class objcls, QObject *parent = nullptr);

signals:

private:
    const Objdet::Class cmClass=Objdet::$nullClass;
};
