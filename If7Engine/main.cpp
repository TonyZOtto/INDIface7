#include "IfSearchApplication.h"
#include "IfSearchWindow.h"

#include "VersionInfo.h"
#include "version.h"

int main(int argc, char *argv[])
{
    IfSearchApplication app(argc, argv);
    VersionInfo vi(VER_MAJOR, VER_MINOR, VER_BRANCH, VER_RELEASE,
                   VER_STRING, VER_COPYRIGHT, VER_ORGNAME, VER_APPNAME);
    app.setOrganizationName(vi.getOrgName());
    app.setApplicationName(vi.getAppName());
    app.setApplicationVersion(vi.toString());
    app.parseOptions(&app);
    IfSearchWindow window(&app);
    app.show(&window);
    return app.exec();
}
