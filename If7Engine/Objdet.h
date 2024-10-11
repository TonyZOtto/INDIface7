#pragma once

#include <QObject>

#include <VersionInfo.h>

class Objdet : public QObject
{
    Q_OBJECT
public: // types
    enum Class
    {
        $nullClass = 0,
        FaceFrontal,
        FaceProfileLeft,
        FaceProfileRight,
        EyeEither,
        EyeLeft,
        EyeRight,
        EyeBoth,
        $maxClass
    };
    Q_ENUM(Class)

public: // ctors
    explicit Objdet(QObject *parent = nullptr);

public slots:

signals:

public: // const
    VersionInfo cvVersion() const;

public: // non-const

private:

};
