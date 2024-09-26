/** @file SettingProperty.cpp
*	Definition of Setting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "SettingProperty.h"

#include <QTimer>

#include "Settings.h"

SettingProperty::SettingProperty(Settings * pSettings,
								 QObject * Object, 
								 const QString & Key, 
								 const QString & PropertyName,
								 int F)
    : QObject(pSettings)
    , settings(pSettings)
    , object(Object)
    , key(Key)
    , propertyName(PropertyName)
    , flags(F)
{
	QObject::connect(Object, SIGNAL(destroyed(QObject *)), 
                     pSettings, SLOT(objectDestroyed(QObject *)));
}

void SettingProperty::setValue(const QVariant & newValue)
{
	if (object->property(qPrintable(propertyName)) != newValue)
	{
		object->setProperty(qPrintable(propertyName), newValue);
		flags |= Settings::Dirty | Settings::Changed;
		value = newValue;
		settings->emitPropertyChanged(key);
	}
}
