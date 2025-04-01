#include "IfSearchWindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
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
    , mpFaceLayout(new QGridLayout)
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
    mpCentralGrid->addLayout(mpFaceLayout, 1, 0, Qt::AlignHCenter);
    mpCentralGrid->addWidget(mpMessageWidget, 2, 0, Qt::AlignLeft);
    mpMessageWidget->setMinimumSize(1200, 800 - 640 - 40);
    mpMessageWidget->setFontFamily("courier");
    Q_ASSERT(mpFrameGrid);
    mpFrameGrid->setRowMinimumHeight(0, maxFrameDim());
    mpFrameGrid->setColumnMinimumWidth(0, maxFrameDim());
    mpFrameGrid->setColumnMinimumWidth(1, maxFrameDim());
    mpFaceLayout->setRowMinimumHeight(0, faceThumbSize().height());
    mpFrameGrid->addWidget(mpMarkedLabel, 0, 0,
                           Qt::AlignTop | Qt::AlignHCenter);
    mpFrameGrid->addWidget(mpDetectLabel, 0, 1,
                           Qt::AlignTop | Qt::AlignHCenter);
#if 0
    QImage tEircImage(":/png/doc/art/logos/EclipseIRLogo.png");
    QImage tIndiImage(":/png/doc/art/logos/INDI200.png");
    QImage tFaceImage(":/png/doc/art/RonOtto1952Thumb.png");
    setMarked(tEircImage);
    setDetect(tIndiImage);
    appendFace(tFaceImage);
#endif
    setMessage(app()->parser().helpText());
}

void IfSearchWindow::clearPixmaps()
{
    setMarked(QImage());
    setDetect(QImage());
    clearFacePixmaps();
    update();
}

void IfSearchWindow::clearFacePixmaps()
{
    for (int col = 0; col < mpFaceLayout->columnCount(); ++col)
        mpFaceLayout->removeItem(mpFaceLayout->itemAtPosition(0, col));
    mFacePixmaps.clear();
}

void IfSearchWindow::setMarked(const QImage &img)
{
    mMarkedPixmap = scaledPixmap(img, maxFrameDim());
    mpMarkedLabel->setPixmap(mMarkedPixmap);
}

void IfSearchWindow::setDetect(const QImage &img)
{
    mDetectPixmap = scaledPixmap(img, maxFrameDim());
    mpDetectLabel->setPixmap(mDetectPixmap);
}

void IfSearchWindow::appendFace(const QImage &img,
                                const QImage &eyesImage,
                                const QImage &normImage)
{
    const int cColumn = mFacePixmaps.count();
    const QPixmap cFacePixmap = scaledPixmap(img, faceThumbSize().width());
    const QPixmap cEyesPixmap = QPixmap::fromImage(eyesImage);
    const QPixmap cNormPixmap = QPixmap::fromImage(normImage);
    mFacePixmaps.append(cFacePixmap);
    mEyesPixmaps.append(cEyesPixmap);
    mNormPixmaps.append(cNormPixmap);
    QLabel * pFaceLabel = new QLabel;
    QLabel * pEyesLabel = new QLabel;
    QLabel * pNormLabel = new QLabel;
    Q_ASSERT(pFaceLabel);
    Q_ASSERT(pEyesLabel);
    Q_ASSERT(pNormLabel);
    pFaceLabel->setPixmap(cFacePixmap);
    pEyesLabel->setPixmap(cEyesPixmap);
    pNormLabel->setPixmap(cEyesPixmap);
    mpFaceLayout->addWidget(pFaceLabel, 0, cColumn);
    mpFaceLayout->addWidget(pFaceLabel, 1, cColumn);
    mpFaceLayout->addWidget(pNormLabel, 2, cColumn);
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

