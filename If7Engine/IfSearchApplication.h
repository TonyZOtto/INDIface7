#pragma once

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>

class QWidget;

#include "VersionInfo.h"

class IfSearchEngine;
class IfSearchWindow;

class IfSearchApplication : public QApplication
{
    Q_OBJECT
public: // types
    enum ShowOption
    {
        $null = 0, Minimized, Normalized, Maximized, FullScreen, $max
    };
    struct Options
    {
        QDir        inputDir = QDir("./input");
        QDir        baseOutputDir = QDir("./output/@");
//        bool        supressMarked = false;
        int         minQuality = 500;
        int         sampleMsec = 1000;
        int         waitingMsec = 60000;
        ShowOption  show = Normalized;
        bool        loop = false;
        bool        deleteAfter = false;
        bool        finishedQuit = false;
        QFileInfo   detectorsXmlFI = QFileInfo("./detectors/Detectors.XML");
        QString     frontalDetectorName;
        int         frontalFactor = 100;
        QDir        markedDir = QDir("./Marked");
        QDir        noFaceDir = QDir("./NoFace");
        QDir        detectedFacesDir = QDir("./DetectedFaces");
        QDir        frontalObjdetDir = QDir("./FrontalObjdet");
        QFileInfo   logFI = QFileInfo("./IfSearch.log");
    };

public: // ctors
    IfSearchApplication(int &argc, char **argv,
                        const VersionInfo vi);

public slots:
    void show(IfSearchWindow *wgt);
    void start();

public: // const
    VersionInfo version() const;
    QFileInfo exeFileInfo() const;
    Options options() const;

public: // non-const
    Options & options();
    QCommandLineParser & parser();
    void setupOptions();
    void parseOptions(QApplication * app);

public: // pointers
    IfSearchWindow * win();

public: // static
    static Options defaultOptions();

private:
    const VersionInfo cmVersion;
    IfSearchWindow * mpWindow=nullptr;
    IfSearchEngine * mpEngine=nullptr;
    QCommandLineParser mParser;
    Options mOptions;
};

inline VersionInfo IfSearchApplication::version() const { return cmVersion; }
inline IfSearchApplication::Options IfSearchApplication::options() const { return mOptions; }
inline IfSearchApplication::Options IfSearchApplication::defaultOptions() { return Options(); }
inline IfSearchApplication::Options & IfSearchApplication::options() { return mOptions; }
inline QCommandLineParser & IfSearchApplication::parser() { return mParser; }
inline IfSearchWindow *IfSearchApplication::win() { return mpWindow; }


