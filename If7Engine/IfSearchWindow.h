#pragma once

#include <QMainWindow>

#include <QPixmap>

class QGridLayout;
class QImage;
class QLabel;
class QSize;
class QTextEdit;
class QWidget;

class IfSearchApplication;

class IfSearchWindow : public QMainWindow
{
    Q_OBJECT
public: // ctors
    explicit IfSearchWindow(IfSearchApplication *ifsApp = nullptr);

public slots:
    void setup();
    void clear(); // private?
    void show();
    void setMarked(const QImage &img);
    void setDetect(const QImage &img);
    void appendFace(const QImage &img);
    void setMessage(const QString &s);

signals:

public: // const

public: // non-const

public: // pointers
    IfSearchApplication * app();

private: // static
    static int maxFrameDim();
    static QSize faceThumbSize();
    static QPixmap scaledPixmap(const QImage &img);

private:
    IfSearchApplication * mpIfsApp=nullptr;
    QWidget * mpCentralWidget=nullptr;
    QGridLayout * mpCentralGrid=nullptr;
    QGridLayout * mpFrameGrid=nullptr;
    QGridLayout * mpFaceGrid=nullptr;
    QPixmap mMarkedPixmap;
    QPixmap mDetectPixmap;
    QLabel * mpMarkedLabel;
    QLabel * mpDetectLabel;
    QList<QPixmap> mFacePixmaps;
    QList<QLabel *> mFaceLabels;
    QTextEdit * mpMessageWidget=nullptr;
};

inline IfSearchApplication *IfSearchWindow::app() { Q_ASSERT(mpIfsApp); return mpIfsApp; }
inline int IfSearchWindow::maxFrameDim() { return 512; }
inline QSize IfSearchWindow::faceThumbSize() { return QSize(128, 128); }


