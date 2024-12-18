#include "IfSearchApplication.h"
#include "IfSearchWindow.h"

int main(int argc, char *argv[])
{
    IfSearchApplication app(argc, argv);
    app.parseOptions(&app);
    IfSearchWindow window;
    window.showMaximized();
    return app.exec();
}
