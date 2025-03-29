#include "IfSearchApplication.h"

#include <QCommandLineOption>
#include <QStringList>
#include <QWidget>

#include "IfSearchEngine.h"
#include "IfSearchWindow.h"

IfSearchApplication::IfSearchApplication(int &argc, char **argv,
                                         const VersionInfo vi)
    : QApplication(argc, argv)
    , cmVersion(vi)
{
    setObjectName("IfSearchApplication");
    setOrganizationName(version().getOrgName());
    setApplicationName(version().getAppName());
    setApplicationVersion(version().toString());
}

void IfSearchApplication::show(IfSearchWindow *wgt)
{
    mpWindow = wgt;
    if (options().show <= $null || options().show >= $max)
        options().show = Normalized;
    qInfo() << Q_FUNC_INFO << options().show;
    switch (options().show)
    {
    case Minimized:    win()->showMinimized();      break;
    case Normalized:   win()->show();               break;
    case Maximized:    win()->showMaximized();      break;
    case FullScreen:   win()->showFullScreen();     break;
    case $null: case $max: default: Q_ASSERT("options().show");
    }
}

void IfSearchApplication::start()
{
    setupOptions();
    parseOptions(this);
    mpEngine = new IfSearchEngine(this);
}

QFileInfo IfSearchApplication::exeFileInfo() const
{
    const QStringList cArgs = QApplication::arguments();
    return QFileInfo(cArgs.first());
}

void IfSearchApplication::setupOptions()
{
    qInfo() << Q_FUNC_INFO;
    parser().setApplicationDescription("IfSearch Engine");
    parser().addHelpOption();
    parser().addVersionOption();
    parser().addPositionalArgument("InputDirectory",
                                   "Directory containing input images [default=./Input]");
    parser().addPositionalArgument("BaseOutputDirectory",
                                   "Destination Base Directory for Output Image Directories (@=timestamp) [default=./Output/@]");
    parser().addOption({"showmin", "Minimize Window."});
    parser().addOption({"shownorm", "Show Normal Window. [default]"});
    parser().addOption({"showmax", "Show Maximized Window."});
    parser().addOption({"showfull", "Show Full Screen."});
    parser().addOption({{"s", "supressNoFaceMarked"},
                       "Don't Output Marked Image If No Faces Detected."});
    parser().addOption({{"q", "minQuality"},
                        "Set Minimum Detected Face Quality. [default 500]",
                        "100~900",
                        QString::number(defaultOptions().minQuality)});
    parser().addOption({{"z", "sampleMsec"},
                        "Process Input Directory Timer [default 1000]",
                        "0~60000",
                        QString::number(defaultOptions().sampleMsec)});
    parser().addOption({{"w", "waitingMsec"},
                        "Waiting for Input Directory Timer [default 60000]",
                        "0~600000",
                        QString::number(defaultOptions().waitingMsec)});
    parser().addOption({{"l", "loop"},
                        "Reprocess Input Directory"});
    parser().addOption({{"a", "deleteAfter"},
                        "Delete Images from Input Directory as Processed"});
    parser().addOption({{"e", "finishedQuit"},
                        "Quit after Processing Input Directory"});
    parser().addOption({{"c", "detectorsXml"},
                       "Specify Detector Catalog. [default ./detectors/Detectors.XML]"
                       "filepath or blank",
                       defaultOptions().detectorsXmlFI.filePath()});
    parser().addOption({{"x", "frontalDetectorName"},
                        "Select Frontal Detector by Name. [default class default]"
                        "name",
                        defaultOptions().frontalDetectorName});
    parser().addOption({{"f", "frontalFactor"},
                       "Set Frontal Detector Density Factor. [default 100]",
                       "10~5000",
                       QString::number(defaultOptions().frontalFactor)});
    parser().addOption({{"m", "markedDir"},
                       "Specify Marked Directory Name.",
                       "directory name",
                       defaultOptions().markedDir.path()});
    parser().addOption({{"n", "noFaceDir"},
                       "Specify No Faces Detected Directory Name.",
                       "directory name",
                       defaultOptions().noFaceDir.path()});
    parser().addOption({{"d", "detectedFacesDir"},
                       "Specify Detected Faces Directory Name.",
                       "directory name",
                       defaultOptions().detectedFacesDir.path()});
    parser().addOption({{"o", "frontalObjdetDir"},
                       "Specify Diagnostic Frontal Face Object Detection Directory Name.",
                       "directory name",
                       defaultOptions().frontalObjdetDir.path()});
    parser().addOption({{"g", "logFile"},
                       "Specify Destination File Name for Logging. [default ./IfSearch.log]",
                       "file name",
                       defaultOptions().logFI.filePath()});
}

void IfSearchApplication::parseOptions(QApplication *app)
{
    qInfo() << Q_FUNC_INFO;
    parser().process(*app);

    const QStringList cPositionalArgs = parser().positionalArguments();
    if (cPositionalArgs.count() > 0)
        options().inputDir = QDir(cPositionalArgs.at(0));
    if (cPositionalArgs.count() > 1)
        options().baseOutputDir = QDir(cPositionalArgs.at(1));

//    options().supressMarked = parser().isSet("supressMarked");
    const int cMinQuality = parser().value("minQuality").toInt();
    if (cMinQuality >= 100 && cMinQuality < 1000)
        options().minQuality = cMinQuality;
    const int cSampleMsec = parser().value("sampleMsec").toInt();
    if (cSampleMsec >= 0 && cSampleMsec < 60000)
        options().sampleMsec = cSampleMsec;
    const int cWaitingMsec = parser().value("waitingMsec").toInt();
    if (cWaitingMsec >= 0 && cWaitingMsec < 600000)
        options().waitingMsec = cWaitingMsec;
    ShowOption tShow = $null;
    if (parser().isSet("showmin")) tShow = Minimized;
    else if (parser().isSet("shownorm")) tShow = Normalized;
    else if (parser().isSet("showmax")) tShow = Maximized;
    else if (parser().isSet("showfull")) tShow = FullScreen;
    options().show = tShow;
    options().loop = parser().isSet("loop");
    options().deleteAfter = parser().isSet("deleteAfter");
    options().finishedQuit = parser().isSet("finishedQuit");
    options().detectorsXmlFI = QFileInfo(parser().value("detectorsXml"));
    options().frontalDetectorName = parser().value("frontalDetectorName");
    const int cFrontalFactor = parser().value("sampleMsec").toInt();
    if (cFrontalFactor >= 10 && cFrontalFactor < 5000)
        options().frontalFactor = cFrontalFactor;
    options().markedDir = QDir(options().baseOutputDir.path() + "./Marked");
    options().noFaceDir = QDir(options().baseOutputDir.path() + "./NoFace");
    options().detectedFacesDir = QDir(options().baseOutputDir.path() + "./DetectedFaces");
    options().frontalObjdetDir = QDir(options().baseOutputDir.path() + "./FrontalObjdet");
    options().logFI = QFileInfo(options().baseOutputDir, parser().value("logFile"));
}

