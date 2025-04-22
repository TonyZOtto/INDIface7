#include "IfSearchWindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QRect>
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
    qDebug() << mpFaceLayout->columnCount();
    mpFrameGrid->addWidget(mpMarkedLabel, 0, 0,
                           Qt::AlignTop | Qt::AlignHCenter);
    mpFrameGrid->addWidget(mpDetectLabel, 0, 1,
                           Qt::AlignTop | Qt::AlignHCenter);
#if 1
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
    qInfo() << Q_FUNC_INFO;
    setMarked(QImage());
    setDetect(QImage());
    clearFacePixmaps();
    update();
}

void IfSearchWindow::clearFacePixmaps()
{
    qInfo() << Q_FUNC_INFO << mFacePixmaps.count()
            << mpFaceLayout->count();
    for (int ix = 0; ix < mpFaceLayout->count(); ++ix)
        mpFaceLayout->removeItem(mpFaceLayout->itemAt(ix));
    mFacePixmaps.clear();
}

void IfSearchWindow::setMarked(const QImage &img)
{
    qInfo() << Q_FUNC_INFO << img;
    mMarkedPixmap = scaledPixmap(img, maxFrameDim());
    mpMarkedLabel->setPixmap(mMarkedPixmap);
}

void IfSearchWindow::setDetect(const QImage &img)
{
    qInfo() << Q_FUNC_INFO << img;
    mDetectPixmap = scaledPixmap(img, maxFrameDim());
    mpDetectLabel->setPixmap(mDetectPixmap);
}

void IfSearchWindow::appendFace(const QImage &img)
{
    const int cColumn = mFacePixmaps.count();
    qInfo() << Q_FUNC_INFO << img.size() << cColumn
            << mpFaceLayout->columnCount();
    const QPixmap cFacePixmap = QPixmap::fromImage(img);
    mFacePixmaps.append(cFacePixmap);
    QLabel * pFaceLabel = new QLabel;
    Q_ASSERT(pFaceLabel);
    pFaceLabel->setPixmap(cFacePixmap);
    mpFaceLayout->addWidget(pFaceLabel, 0, cColumn);
}

void IfSearchWindow::appendEyes(const QImage &eyeLImage,
                                const QImage &eyeRImage)
{
    const int cColumn = mEyesPixmaps.count();
    static const int scThumbWidth = faceThumbSize().width();
    QPainter tPainter;
    QPixmap tEyesPixmap;
    tPainter.begin(&tEyesPixmap);
    tPainter.drawImage(0, 0, eyeLImage);
    tPainter.drawImage(scThumbWidth / 2, 0, eyeRImage);
    tPainter.end();
    qInfo() << Q_FUNC_INFO << eyeLImage << eyeRImage << tEyesPixmap;
    mEyesPixmaps.append(tEyesPixmap);
    QLabel * pEyesLabel = new QLabel;
    Q_ASSERT(pEyesLabel);
    pEyesLabel->setPixmap(tEyesPixmap);
    mpFaceLayout->addWidget(pEyesLabel, 1, cColumn);
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
    if (img.isNull())
        result = QPixmap(dim, dim);
    else if (img.width() > img.height())
        result = QPixmap::fromImage(img.scaledToWidth(dim));
    else
        result = QPixmap::fromImage(img.scaledToHeight(dim));
    qInfo() << Q_FUNC_INFO << dim << img << result;
    return result;
}

