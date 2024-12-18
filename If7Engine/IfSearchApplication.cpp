#include "IfSearchApplication.h"

#include <QCommandLineParser>

IfSearchApplication::IfSearchApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , cmVersion(VER_MAJOR, VER_MINOR, VER_BRANCH, VER_RELEASE,
               VER_STRING, VER_COPYRIGHT, VER_ORGNAME, VER_APPNAME)
{
    setObjectName("IfSearchApplication");
    parseOptions(this);
}

void IfSearchApplication::parseOptions(QApplication *app)
{
    ShowOption tShow = $null;
    QCommandLineParser tParser;
    tParser.setApplicationDescription("IfSearch Engine");
    tParser.addHelpOption();
    tParser.addVersionOption();
    tParser.addPositionalArgument("logfile",
                                 "Destination File for Logging.");
    tParser.addOption({{"n", "showmin"}, "Show Minimized Log Window."});
    tParser.addOption({{"m", "shownorm"}, "Show Normal Log Window."});
    tParser.addOption({{"x", "showmax"}, "Show Maximized Log Window."});
    tParser.process(*app);

    if (tParser.isSet("showmin")) tShow = Minimized;
    else if (tParser.isSet("shownorm")) tShow = Normal;
    else if (tParser.isSet("showmax")) tShow = Maximized;
    else tShow = Defalt;

    mShowOption = tShow;
    //mLogFI = QFileInfo(tParser.positionalArguments().first());
}

