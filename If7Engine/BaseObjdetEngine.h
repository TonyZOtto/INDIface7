#pragma once

#include <QObject>
#include "Objdet.h"

class BaseObjdetEngine : public Objdet
{
    Q_OBJECT
public: // ctors
    explicit BaseObjdetEngine(QObject *parent = nullptr);
protected:
    BaseObjdetEngine(const Objdet::Class objcls, QObject *parent = nullptr);

signals:


public: // static
    QString className(const Objdet::Class odc) const;

private:
    const Objdet::Class cmClass=Objdet::$nullClass;
};
