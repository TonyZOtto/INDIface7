#pragma once

#include <QObject>

class FrontalMarker;
class FrontalObjdet;

class FrameObjdet : public QObject
{
    Q_OBJECT
public:
    explicit FrameObjdet(QObject *parent = nullptr);

signals:

private:
    FrontalObjdet * mpObjdet=nullptr;
    FrontalMarker * mpMarker=nullptr;
};
