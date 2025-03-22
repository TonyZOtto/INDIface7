#include "IfSearchWindow.h"

#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

#include "IfSearchApplication.h"

IfSearchWindow::IfSearchWindow(IfSearchApplication *ifsApp)
    : QMainWindow{nullptr}
    , mpIfsApp(ifsApp)
    , mpCentralWidget(new QWidget)
    , mpCentralGrid(new QGridLayout)
    , mpFrameGrid(new QGridLayout)
    , mpFaceGrid(new QGridLayout)
    , mpMarkedLabel(new QLabel)
    , mpDetectLabel(new QLabel)
    , mpMessageWidget(new QTextEdit)
{
    setObjectName("IfSearchWindow");
    QTimer::singleShot(100, this, &IfSearchWindow::setup);
}

void IfSearchWindow::setup()
{
    qInfo() << Q_FUNC_INFO;
    Q_ASSERT(mpCentralWidget); Q_ASSERT(mpCentralGrid);
    setMinimumSize(1280, 800);
    mpCentralWidget->setLayout(mpCentralGrid);
    setCentralWidget(mpCentralWidget);
    mpCentralGrid->addLayout(mpFrameGrid, 0, 0, Qt::AlignHCenter);
    mpCentralGrid->addLayout(mpFaceGrid, 1, 0, Qt::AlignHCenter);
    mpCentralGrid->addWidget(mpMessageWidget, 2, 0, Qt::AlignLeft);
    mpMessageWidget->setMinimumSize(1200, 800 - 640 - 40);
    Q_ASSERT(mpFrameGrid);
    mpFrameGrid->setRowMinimumHeight(0, maxFrameDim());
    mpFrameGrid->setColumnMinimumWidth(0, maxFrameDim());
    mpFrameGrid->setColumnMinimumWidth(1, maxFrameDim());
    mpFrameGrid->addWidget(mpMarkedLabel, 0, 0,
                           Qt::AlignTop | Qt::AlignHCenter);
    mpFrameGrid->addWidget(mpDetectLabel, 0, 1,
                           Qt::AlignTop | Qt::AlignHCenter);
    mpFaceGrid->setRowMinimumHeight(0, faceThumbSize().height());

    QImage tEircImage(":/png/doc/art/logos/EclipseIRLogo.png");
    QImage tIndiImage(":/png/doc/art/logos/INDI200.png");
    QImage tFaceImage(":/png/doc/art/RonOtto1952Thumb.png");
    setMarked(tEircImage);
    setDetect(tIndiImage);
    appendFace(tFaceImage);
    setMessage(app()->parser().helpText());
    QTimer::singleShot(100, this, &IfSearchWindow::update);
}

void IfSearchWindow::clear()
{
    setMarked(QImage());
    setDetect(QImage());
    for (int ix = 0; ix < mFaceLabels.count(); ++ix)
    {
        QLabel * pLabel = mFaceLabels.at(ix);
        mpFaceGrid->removeWidget(pLabel);
        pLabel->deleteLater();
    }
    mFacePixmaps.clear();
    mFaceLabels.clear();
}

void IfSearchWindow::update()
{
    qInfo() << Q_FUNC_INFO;
    Q_ASSERT(mpMarkedLabel); Q_ASSERT(mpDetectLabel);
    mpMarkedLabel->setPixmap(mMarkedPixmap);
    mpDetectLabel->setPixmap(mDetectPixmap);
    Q_ASSERT(mFaceLabels.isEmpty());
    foreach (const QPixmap cPixmap, mFacePixmaps)
    {
        QLabel * pFaceLabel = new QLabel;
        Q_ASSERT(pFaceLabel);
        pFaceLabel->setPixmap(cPixmap);
        mpFaceGrid->addWidget(pFaceLabel);
    }
}

void IfSearchWindow::setMarked(const QImage &img)
{
    mMarkedPixmap = scaledPixmap(img, maxFrameDim());
}

void IfSearchWindow::setDetect(const QImage &img)
{
    mDetectPixmap = scaledPixmap(img, maxFrameDim());
}

void IfSearchWindow::appendFace(const QImage &img)
{
    mFacePixmaps.append(scaledPixmap(img, 128));
}

void IfSearchWindow::setMessage(const QString &s)
{
    Q_ASSERT(mpMessageWidget);
    mpMessageWidget->clear();
    mpMessageWidget->setText(s);
}

/* ------------------------ static ------------------------- */

QPixmap IfSearchWindow::scaledPixmap(const QImage &img,
                                     const int dim)
{
    QPixmap result;
    if (img.width() > img.height())
        result = QPixmap::fromImage(img.scaledToWidth(dim));
    else
        result = QPixmap::fromImage(img.scaledToHeight(dim));
    return result;
}

