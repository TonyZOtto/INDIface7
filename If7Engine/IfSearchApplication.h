#pragma once

#include <QApplication>
#include <QFileInfo>
#include <QWidget>

#include "VersionInfo.h"
#include "version.h"

class IfSearchApplication : public QApplication
{
    Q_OBJECT
public: // types
    enum ShowOption
    {
        $null = 0, Minimized, Normal, Maximized, Defalt=Maximized
    };

public: // ctors
    IfSearchApplication(int &argc, char **argv);

public slots:
    void show(QWidget * wgt);

public: // const

public: // non-const
    void parseOptions(QApplication * app);

private:
    const VersionInfo cmVersion;
    ShowOption mShowOption;
    QFileInfo mLogFI;
};

