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
    const FrontalObjdet * cmpObjdet=nullptr;
    const FrontalMarker * cmpMarker=nullptr;
};
