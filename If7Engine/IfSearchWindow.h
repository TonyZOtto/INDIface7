#pragma once

#include <QMainWindow>

class IfSearchApplication;

class IfSearchWindow : public QMainWindow
{
    Q_OBJECT
public: // ctors
    explicit IfSearchWindow(IfSearchApplication *ifsApp = nullptr);

public slots:

signals:

public: // const

public: // non-const

public: // pointers
    IfSearchApplication * app();

private:
    IfSearchApplication * mpIfsApp;
};

inline IfSearchApplication *IfSearchWindow::app() { Q_ASSERT(mpIfsApp); return mpIfsApp; }
