#include "IfSearchWindow.h"

#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QWidget>

#include "IfSearchApplication.h"

IfSearchWindow::IfSearchWindow(IfSearchApplication *ifsApp)
    : QMainWindow{nullptr}
    , mpIfsApp(ifsApp)
{
    setObjectName("IfSearchWindow");
    mpCentralGrid = new QGridLayout;
    mpCentralWidget = new QWidget;
    QTimer::singleShot(100, this, &IfSearchWindow::setup);
}

void IfSearchWindow::setup()
{
    qInfo() << Q_FUNC_INFO;
    setMinimumSize(1280, 800);
    mpCentralWidget->setLayout(mpCentralGrid);
    setCentralWidget(mpCentralWidget);

    QLabel * pEircLabel = new QLabel(this);
    QLabel * pIndiLabel = new QLabel(this);
    QImage tEircImage(":/png/doc/art/logos/EclipseIRLogo.png");
    QImage tIndiImage(":/png/doc/art/logos/INDI200.png");
    tEircImage = tEircImage.scaledToWidth(512);
    tIndiImage = tIndiImage.scaledToWidth(512);
    pEircLabel->setPixmap(QPixmap::fromImage(tEircImage));
    pIndiLabel->setPixmap(QPixmap::fromImage(tIndiImage));
    qInfo() << pEircLabel->pixmap().size()
            << pIndiLabel->pixmap().size();
    mpCentralGrid->addWidget(pEircLabel, 0, 0,
                             Qt::AlignTop | Qt::AlignHCenter);
    mpCentralGrid->addWidget(pIndiLabel, 0, 1,
                             Qt::AlignTop | Qt::AlignHCenter);

}

