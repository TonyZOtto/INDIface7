/** @file Settings.h
*	Public declaration of Settings class for DDT config library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTCFG_LIB
# define DDTCFG_EXPORT Q_DECL_EXPORT
#else
# define DDTCFG_EXPORT Q_DECL_IMPORT
#endif

#include <QFlags>
#include <QList>
#include <QMap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QVariant>
class QDomElement;
class QTimer;
class QReadWriteLock;

class Return;
class StatusHandler;
class VersionInfo;
class Setting;
class SettingItem;
class SettingProperty;
class SettingsScanner;

class Settings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int UpdateMsec READ updateMsec WRITE setUpdateMsec)
    Q_PROPERTY(bool WriteBack READ writeBack WRITE setWriteBack)
    Q_PROPERTY(int PollCount READ pollCount)
    friend class Setting;
    friend class SettingItem;
    friend class SettingsScanner;
    friend class SettingProperty;

public:
    enum Flag
    {
        $null       = 0,
        Volatile	= 0x00000001,
        ReadOnly	= 0x00000002,
        Advanced	= 0x00000004,
        Hidden		= 0x00000008,
        WidgetState	= 0x00000100,
        Geometry	= 0x00000200,
        Dirty		= 0x00010000,
        Changed		= 0x00020000,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

private:
    Settings(QObject * parent = 0);
    Settings(const QString & organization,
             const QString & application = QString(),
             QObject * parent = 0);
    Settings(const QString & iniFilename,
             QObject * parent);

public:
    static Settings * newSettings(QObject * parent = 0);
    ~Settings();

    void appStart(void);
    void setPollCountKey(const QString & key, int count=0);
    void objectProperties(QObject * Object,
                          const QString & BaseKey,
                          const QStringList & PropertyNames,
                          Flags F=$null)
    { foreach(QString n, PropertyNames) objectProperty(Object, BaseKey, n, F); }
    void objectProperties(QObject * Object,
                          const QString & BaseKey,
                          const QList<QByteArray> & PropertyNames,
                          Flags F=$null)
    { foreach(QByteArray n, PropertyNames) objectProperty(Object, BaseKey, n, F); }
    void objectProperty(QObject * Object, const QString & BaseKey,
                        const QString & PropertyName, Flags F=$null);
    void objectProperty(const QString & Key, QObject * Object,
                        const QString & PropertyName, Flags F=$null);
    void startScanner(void);
    void startTimers(void);
    QVariant valueOf(const QString & key) const;
    QString programName(void) const { return arg0; }
    QStringList argList(void) const { return args; }
    int argSize(void) const { return args.size(); }
    QString argTake(void) { return args.size() ? args.takeFirst() : QString(); }
    QString arg(int x) { return (x < 0 || x > args.size()-1) ? QString() : args.at(x); }
    void dump(const QString & prefix=QString());
    bool clearGroup(const QString & groupName);
    QString source(void) const;

public: // overrides
    QVariant value(const QString & key, const QVariant defaultValue=QVariant()) const;
    void setValue(const QString & key, const QVariant newValue);

private:
    void construct(Setting * child);
    void destruct(Setting * child);
    void emitValueChanged(QString key);
    void emitPropertyChanged(QString key);
    QString flagsString(int f);
    void setVersion(const VersionInfo & ver, const QString & key);

public:
    bool writeBack(void) { return WriteBack; }
    int updateMsec(void) { return UpdateMsec; }
    int pollCount(void)	{ return PollCount; }
    void setWriteBack(bool b) { WriteBack = b; }
    void setUpdateMsec(int ms);

public slots:
    void scanForUpdate(void);
    void scan(void);
    void updatePollCount(void);

private slots:
    void objectDestroyed(QObject * Object);
    void changeProperty(QString key, QVariant var);

signals:
    void valueChanged(QString key);
    void propertyChanged(QString key);
    void scanStart(void);
    void scanFinish(void);

private:
    QMap<QString, Setting *> mSettingMap;
    QMultiMap<QString, SettingProperty *> mProperyMap;
    QReadWriteLock * rwlVars;
    QReadWriteLock * rwlProps;
    bool WriteBack;
    bool Mode;
    int UpdateMsec;
    int PollCount;
    SettingsScanner * mpScanner;
    QTimer * timerUpdate;
    QTimer * timerScan;
    QString keyForUpdateMsec;
    QString keyForPollCount;
    QStringList args;
    QMap<QString,QString> opts;
    QString arg0;
}; // class Settings

Q_DECLARE_OPERATORS_FOR_FLAGS(Settings::Flags)

