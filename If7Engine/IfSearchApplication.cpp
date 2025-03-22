#include "IfSearchApplication.h"

#include <QCommandLineOption>
#include <QWidget>

#include "IfSearchEngine.h"

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

void IfSearchApplication::show(QWidget *wgt)
{
    if (options().show <= $null || options().show >= $max)
        options().show = Normalized;
    switch (options().show)
    {
    case Minimized:    wgt->showMinimized();    break;
    case Normalized:   wgt->show();             break;
    case Maximized:    wgt->showMaximized();    break;
    case $null: case $max: default: Q_ASSERT("options().show");
    }
}

void IfSearchApplication::start()
{
    setupOptions();
    parseOptions(this);
    mpEngine = new IfSearchEngine(this);
}

void IfSearchApplication::setupOptions()
{
    qInfo() << Q_FUNC_INFO;
    parser().setApplicationDescription("IfSearch Engine");
    parser().addHelpOption();
    parser().addVersionOption();
    parser().addOption({"showmin", "Minimize Window."});
    parser().addOption({"shownorm", "Show Normal Window."});
    parser().addOption({"showmax", "Show Maximized Window."});
    parser().addOption({{"q", "minQuality"},
                       "Set Minimum Detected Face Quality.",
                       "<100~900>",
                       QString::number(defaultOptions().minQuality)});
    parser().addOption({{"x", "frontalDetectorName"},
                       "Select Frontal Detector."
                       "<name> or blank",
                       defaultOptions().frontalDetectorName});
    parser().addOption({{"f", "frontalFactor"},
                       "Set Frontal Detector Density Factor.",
                       "<100~4000>",
                       QString::number(defaultOptions().frontalFactor)});
    parser().addOption({{"m", "markedDir"},
                       "Specify Marked Directory Name.",
                       "<directory name>",
                       defaultOptions().markedDir.path()});
    parser().addOption({{"m", "noFaceDir"},
                       "Specify No Faces Detected Directory Name.",
                       "<directory name>",
                       defaultOptions().noFaceDir.path()});
    parser().addOption({{"d", "detectedFaceDir"},
                       "Specify Detected Faces Directory Name.",
                       "<directory name>",
                       defaultOptions().detectedFaceDir.path()});
    parser().addOption({{"o", "frontalObjdetDir"},
                       "Specify Diagnostic Frontal Face Object Detection Directory Name.",
                       "<directory name>",
                       defaultOptions().frontalObjdetDir.path()});
    parser().addOption({{"l", "logFile"},
                       "Specify Destination File Name for Logging.",
                       "<file name>",
                       defaultOptions().logFI.filePath()});
}

void IfSearchApplication::parseOptions(QApplication *app)
{
    qInfo() << Q_FUNC_INFO;
    parser().process(*app);

    ShowOption tShow = $null;
    if (parser().isSet("showmin")) tShow = Minimized;
    else if (parser().isSet("shownorm")) tShow = Normalized;
    else if (parser().isSet("showmax")) tShow = Maximized;
    options().show = tShow;

    qDebug() << parser().value("minQuality");
}

