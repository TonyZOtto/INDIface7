#pragma once

#include <QApplication>
#include <QWidget>

#include "VersionInfo.h"
#include "version.h"

class IfSearchApplication : public QApplication
{
    Q_OBJECT
public:
    IfSearchApplication(int &argc, char **argv);

private:
    const VersionInfo cmVersion;
};
