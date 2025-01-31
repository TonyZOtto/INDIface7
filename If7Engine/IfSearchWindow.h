#pragma once

#include <QMainWindow>

class QGridLayout;
class QWidget;

class IfSearchApplication;

class IfSearchWindow : public QMainWindow
{
    Q_OBJECT
public: // ctors
    explicit IfSearchWindow(IfSearchApplication *ifsApp = nullptr);

public slots:
    void setup();

signals:

public: // const

public: // non-const

public: // pointers
    IfSearchApplication * app();

private:
    IfSearchApplication * mpIfsApp=nullptr;
    QGridLayout * mpCentralGrid=nullptr;
    QWidget * mpCentralWidget=nullptr;
};

inline IfSearchApplication *IfSearchWindow::app() { Q_ASSERT(mpIfsApp); return mpIfsApp; }
