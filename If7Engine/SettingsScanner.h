#pragma once

#include <QThread>

#include <QString>
#include <QVariant>

class Settings;

class SettingsScanner : public QThread
{
    Q_OBJECT
    friend class Setting;
    friend class Settings;

public:
    SettingsScanner(Settings * parent);
    ~SettingsScanner();

protected:
    virtual void run();

signals:
    void changeProperty(QString key, QVariant var);

private:
    Settings * settings;
    bool done;
};
