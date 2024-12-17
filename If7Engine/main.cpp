#include "IfSearchApplication.h"
#include "IfSearchWindow.h"

int main(int argc, char *argv[])
{
    IfSearchApplication app(argc, argv);
    IfSearchWindow window;
    window.showMaximized();
    return app.exec();
}
