#include "IfSearchWindow.h"

#include "IfSearchApplication.h"

IfSearchWindow::IfSearchWindow(IfSearchApplication *ifsApp)
    : QMainWindow{nullptr}
    , mpIfsApp(ifsApp)
{
    setObjectName("IfSearchWindow");
    setMinimumSize(1280, 960);
}

