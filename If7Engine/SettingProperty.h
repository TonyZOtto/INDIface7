#pragma once

#include <QObject>

#include <QString>
#include <QVariant>

class Settings;

class SettingProperty : public QObject
{
    Q_OBJECT
    friend class Settings;
    friend class SettingsScanner;

public:
    SettingProperty(Settings * pSettings, QObject * Object,
                    const QString & Key, const QString & PropertyName,
                    int F=0);
    void setValue(const QVariant & newValue);

    //	private:
public:
    Settings *	settings;
    QObject * object;
    QString key;
    QString propertyName;
    QVariant value;
    int flags;
}; // class SettingProperty
