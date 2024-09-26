/** @file Settings.cpp
*	Definition of Settings class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "Settings.h"

#include <QCoreApplication>
#include <QMutableMapIterator>
#include <QTimer>

#include "Setting.h"
#include "SettingProperty.h"
#include "SettingsScanner.h"

Settings::Settings(QObject * parent)
    : QSettings(parent)
    , mpScanner(0)
    , timerUpdate(0)
    , timerScan(0)
{
    PollCount = 0;
    UpdateMsec = 0;
    WriteBack = false;
    AdvancedMode = false;
}

Settings::Settings(const QString & organization,
                   const QString & application,
                   QObject * parent)
    : QSettings(organization, application, parent)
    , mpScanner(0)
    , timerUpdate(0)
    , timerScan(0)
{
    PollCount = 0;
    UpdateMsec = 0;
    WriteBack = false;
    AdvancedMode = false;
}

Settings::Settings(const QString & iniFilename,
                    QObject * parent)
    : QSettings(iniFilename, QSettings::IniFormat, parent)
    , mpScanner(0)
    , timerUpdate(0)
    , timerScan(0)
{
    PollCount = 0;
    UpdateMsec = 0;
    WriteBack = false;
    AdvancedMode = false;
}


Settings::~Settings()
{
    delete timerScan;
    delete timerUpdate;

    if (mpScanner)
    {
        mpScanner->done = true;
        mpScanner->wait(11000);
        if ( ! mpScanner->isFinished())
            mpScanner->terminate();
    }
}

Settings * Settings::newSettings(QObject * parent)
{
    QString organization(qApp ? qApp->organizationName() : QString());
    QString application(qApp ? qApp->applicationName() : QString());

    Settings * instance = 0;

    // Scan Applications argument list
    QStringList argList = qApp->arguments();
    QString arg0 = argList.takeFirst();
    QStringList args;
    QString iniFilename;
    QMap<QString,QString> opts;

    foreach (QString arg, argList)
    {
        if (arg.startsWith(QChar('/')))
        {
            int x = arg.indexOf(QChar('='));
            if (x < 0)
                opts.insert(arg.mid(1).toLower(), QString());
            else
            {
                QString opt = arg.mid(1, x-1);
                QString val = arg.mid(x+1);
                opts.insert(opt.toLower(), val);
            }
        }
        else if (arg.startsWith(QChar('%'))
                 || arg.startsWith('_'))
        {
            int x = arg.indexOf(QChar('/'));
            if (x < 0)
                application = arg.mid(1);
            else
            {
                organization = arg.mid(1, x-1);
                application = arg.mid(x+1);
            }
        }
        else if (arg.startsWith('@'))
        {
            iniFilename = arg.mid(1);
        }
        else
        {
            args.append(arg);
        }
    } // foreach(arg)

    if (iniFilename.isEmpty())
        instance = new Settings(organization, application, parent);
    else
        instance = new Settings(iniFilename);
    if (instance)
    {
        instance->arg0 = arg0;
        instance->args = args;
        instance->opts = opts;
    }
    return instance;
}

void Settings::startTimers(void)
{
    if (timerUpdate)
    {
        delete timerUpdate;
        timerUpdate = 0;
    }
    if (timerScan)
    {
        delete timerScan;
        timerScan = 0;
    }

    timerUpdate = new QTimer(this);
    timerScan = new QTimer(this);

    timerUpdate->setInterval(20000);
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(scanForUpdate()));

    timerScan->setInterval(UpdateMsec);
    if (UpdateMsec)
    {
        connect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
        timerScan->start();
    }

    timerUpdate->start();
} // startTimers()

void Settings::setUpdateMsec(int ms)
{
    if (timerScan)
    {
        disconnect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
        timerScan->stop();

        timerScan->setInterval(ms);

        if (ms)
        {
            connect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
            timerScan->start();
        }
    }
    UpdateMsec = ms;
}

void Settings::setPollCountKey(const QString & key, int count)
{

    if (key.isEmpty())
        disconnect(this, SIGNAL(scanFinish()), this, SLOT(updatePollCount()));
    else
        connect(this, SIGNAL(scanFinish()), this, SLOT(updatePollCount()));
    keyForPollCount = key;
    PollCount = count;
    updatePollCount();
}

void Settings::construct(Setting * child)
{
    mSettingMap[child->key.toLower()] = child;
    QVariant def = *child;
    QVariant var = value(child->key, def);
    *(QVariant *)child = var;
    QString key = child->key.toLower();
    if (opts.contains(key))
    {
        *(QVariant *)child = opts.value(key);
        opts.remove(key);
        child->flags |= Settings::Dirty | Settings::Changed;
    }
} // construct()

void Settings::objectProperty(QObject * Object, const QString & BaseKey, const QString & PropertyName, Flags F)
{
    QString key = BaseKey + (BaseKey.isEmpty() ? "" : "/") + PropertyName;
    objectProperty(key, Object, PropertyName, F);
} // objectProperty(basekey)

void Settings::objectProperty(const QString & Key, QObject * Object, const QString & PropertyName, Flags F)
{
    SettingProperty * child = new SettingProperty(this, Object, Key, PropertyName, F);
    mProperyMap.insert(Key.toLower(), child);
    QVariant def = Object->property(qPrintable(PropertyName));
    QVariant var = value(Key, def);
    child->value = var;
    if (var != def)
        Object->setProperty(qPrintable(PropertyName), var);
    QString mapKey = Key.toLower();
    if (opts.contains(mapKey))
    {
        Object->setProperty(qPrintable(PropertyName), opts.value(mapKey));
        opts.remove(mapKey);
        child->flags |= Settings::Dirty | Settings::Changed;
    }
    if (this == Object && 0 == PropertyName.compare(tr("UpdateMsec", "config"), Qt::CaseInsensitive))
        keyForUpdateMsec = Key;
} // objectProperty(key)

void Settings::destruct(Setting * child)
{
    if (WriteBack
        && ! child->flags.testFlag(ReadOnly)
        && child->flags.testFlag(Dirty))
    {
        QVariant var = *child;
        setValue(child->key, var.toString());
    }
    mSettingMap.remove(child->key.toLower());
} // destruct()

void Settings::objectDestroyed(QObject * Object)
{
    foreach (SettingProperty * pProp, mProperyMap.values())
    {
        if (pProp->object == Object)
        {
            if (WriteBack
                && ! (pProp->flags & ReadOnly)
                && (pProp->flags & Dirty))
            {
                QVariant var = pProp->object->property(qPrintable(pProp->propertyName));
                setValue(pProp->key, var.toString());
            }
            pProp->deleteLater();
            mProperyMap.remove(pProp->key, pProp);
        }
    }
} // objectDestroyed()


QVariant Settings::valueOf(const QString & key) const
{
    QVariant rtn("");
    QString mapKey = key.toLower();

    if (mSettingMap.contains(mapKey))
    {
        Setting * var = mSettingMap[mapKey];
        rtn = *(QVariant *)var;
    }
    else if (mProperyMap.contains(mapKey))
    {
        SettingProperty * prop = mProperyMap.value(mapKey);
        rtn = prop->value;
    }
    else if (opts.contains(mapKey))
    {
        rtn = opts.value(mapKey);
    }

    return rtn;
}


QVariant Settings::value(const QString & key, const QVariant defaultValue) const
{
    QVariant result;
    QString mapKey = key.toLower();

    if (opts.contains(mapKey))
        result = opts.value(mapKey);
    else
        result = QSettings::value(key, defaultValue);
    return result;
} // value() override

void Settings::setValue(const QString & key, const QVariant newValue)
{
    if (mSettingMap.contains(key))
    {
        Setting * var = mSettingMap[key];
        //var->setValue(newValue);
        *(QVariant *)var = newValue;
    }
    else if (mProperyMap.contains(key))
    {
        SettingProperty * prop = mProperyMap.value(key);
        //prop->setValue(newValue);
        *(QVariant *)prop = newValue;
    }
    QSettings::setValue(key, newValue);
} // setValue() override

void Settings::startScanner(void)
{
    if (mpScanner)
    {
        if ( ! mpScanner->isRunning())
            mpScanner->start(QThread::LowestPriority);
    }
    else
    {
        mpScanner = new SettingsScanner(this);
        qRegisterMetaType<QVariant>("QVariant");
        connect(mpScanner, SIGNAL(changeProperty(QString,QVariant)),
                this, SLOT(changeProperty(QString,QVariant)));
        mpScanner->start(QThread::LowestPriority);
    }


} // startScanner()


void Settings::changeProperty(QString key, QVariant var)
{
    QList<SettingProperty *> props = mProperyMap.values(key.toLower());
    foreach (SettingProperty * prop, props)
        prop->setValue(var);
} // changeProperty() SLOT




void Settings::emitValueChanged(QString key)
{
    emit valueChanged(key);
}

void Settings::emitPropertyChanged(QString key)
{
    emit propertyChanged(key);
}

void Settings::dump(const QString & prefix)
{
    QString flags, value, objName;

    qInfo() << "---Settings from " << source();
    foreach(Setting * var, mSettingMap)
    {
        if ( ! var->key.startsWith(prefix, Qt::CaseInsensitive))
            continue;
        flags = flagsString(var->flags);
        value = var->toString();
        if (value.isEmpty())
            qInfo() << QString("   [%1] {%2} empty").arg(flags).arg(var->key);
        else
            qInfo() << QString("   [%1] {%2} %3").arg(flags).arg(var->key).arg(value);
    }
    foreach(SettingProperty * prop, mProperyMap)
    {
        if ( ! prop->key.startsWith(prefix, Qt::CaseInsensitive))
            continue;
        flags = flagsString(prop->flags);
        if (prop->object)
            objName = prop->objectName();
        if (objName.isEmpty())
            objName = QString::number((unsigned int)prop->object, 16);
        if (prop->value.isNull())
            qInfo() << QString("   [%1] {%2} for %3 empty")
                    .arg(flags).arg(prop->key).arg(objName);
        else
            qInfo() << QString("   [%1] {%2} for %3 %4")
                    .arg(flags).arg(prop->key).arg(objName).arg(prop->value);
    }
} // dump(sev)

QString Settings::flagsString(int f)
{
    QString rtn;
    rtn += (f & Volatile)	? "V" : " ";
    rtn += (f & ReadOnly)	? "R" : " ";
    rtn += (f & Advanced)	? "A" : " ";
    rtn += (f & Hidden)		? "H" : " ";
    rtn += (f & WidgetState)? "W" : " ";
    rtn += (f & Geometry)	? "G" : " ";
    rtn += (f & Dirty)		? "D" : " ";
    rtn += (f & Changed)	? "C" : " ";
    return rtn;
}

bool Settings::clearGroup(const QString & groupName)
{
    if ( ! childGroups().contains(groupName))
        return false;
    beginGroup(groupName);
    remove(groupName);
    endGroup();
    return true;
}

QString Settings::source(void) const
{
    if (QSettings::fileName().isEmpty())
        return QSettings::organizationName()
                + "/" + QSettings::applicationName();
    else
        return QSettings::fileName();
}
