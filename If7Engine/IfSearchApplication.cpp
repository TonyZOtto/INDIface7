#include "IfSearchApplication.h"

#include <QCommandLineParser>

#include "IfSearchEngine.h"

IfSearchApplication::IfSearchApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , cmVersion(VER_MAJOR, VER_MINOR, VER_BRANCH, VER_RELEASE,
               VER_STRING, VER_COPYRIGHT, VER_ORGNAME, VER_APPNAME)
{
    setObjectName("IfSearchApplication");
    parseOptions(this);
}

void IfSearchApplication::show(QWidget *wgt)
{
    switch (mShowOption)
    {
    case Minimized:    wgt->showMinimized();    break;
    case Normal:       wgt->show();             break;
    case Maximized:    wgt->showMaximized();    break;
    default:           wgt->show();             break;
    }
}

void IfSearchApplication::start()
{
    mpEngine = new IfSearchEngine(this);
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
    if (tParser.positionalArguments().count() > 0)
        mLogFI = QFileInfo(tParser.positionalArguments().at(0));
}
