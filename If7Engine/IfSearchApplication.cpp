#include "IfSearchApplication.h"

IfSearchApplication::IfSearchApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , cmVersion(VER_MAJOR, VER_MINOR, VER_BRANCH, VER_RELEASE,
               VER_STRING, VER_COPYRIGHT, VER_ORGNAME, VER_APPNAME)
{
    setObjectName("IfSearchApplication");
}
