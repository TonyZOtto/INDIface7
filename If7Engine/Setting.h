#pragma once

#include <QVariant>

#include <QString>

#include "Settings.h"

class  Setting : public QVariant
{
    friend class Settings;
    friend class SettingsScanner;

public:
    Setting(Settings * Parent,
            const QString & Key,
            QVariant Default,
            Settings::Flags F=Settings::$null);
    ~Setting();

    QString keyName(void) const { return key; }
    void setValue(const QVariant & value);

private:
    Settings *settings;
    QString key;
    Settings::Flags flags;
}; // class Setting
