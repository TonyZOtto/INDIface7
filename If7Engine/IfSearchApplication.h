#pragma once

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>

class QWidget;

#include "VersionInfo.h"

class IfSearchEngine;

class IfSearchApplication : public QApplication
{
    Q_OBJECT
public: // types
    enum ShowOption
    {
        $null = 0, Minimized, Normalized, Maximized, $max
    };
    struct Options
    {
        ShowOption  show = Normalized;
        bool        supressMarked = false;
        int         minQuality = 500;
        QFileInfo   detectorsXmlFI = QFileInfo("./detectors/Detectors.XML");
        QString     frontalDetectorName;
        int         frontalFactor = 100;
        QDir        inputDir = QDir("./Input");
        QDir        baseOutputDir = QDir("./@");
        QDir        markedDir = QDir(baseOutputDir.path() + "./Marked");
        QDir        noFaceDir = QDir(baseOutputDir.path() + "./NoFace");
        QDir        detectedFaceDir = QDir(baseOutputDir.path() + "./DetectedFace");
        QDir        frontalObjdetDir = QDir(baseOutputDir.path() + "./FrontalObjdet");
        QFileInfo   logFI = QFileInfo(baseOutputDir, "./IfSearch.log");
    };

public: // ctors
    IfSearchApplication(int &argc, char **argv,
                        const VersionInfo vi);

public slots:
    void show(QWidget * wgt);
    void start();

public: // const
    VersionInfo version() const;
    Options options() const;

public: // non-const
    Options & options();
    QCommandLineParser & parser();
    void setupOptions();
    void parseOptions(QApplication * app);

public: // static
    static Options defaultOptions();

private:
    const VersionInfo cmVersion;
    IfSearchEngine * mpEngine=nullptr;
    QCommandLineParser mParser;
    Options mOptions;
};

inline VersionInfo IfSearchApplication::version() const { return cmVersion; }
inline IfSearchApplication::Options IfSearchApplication::options() const { return mOptions; }
inline IfSearchApplication::Options IfSearchApplication::defaultOptions() { return Options(); }
inline IfSearchApplication::Options & IfSearchApplication::options() { return mOptions; }
inline QCommandLineParser & IfSearchApplication::parser() { return mParser; }


