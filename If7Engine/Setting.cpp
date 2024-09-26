/** @file Setting.cpp
*	Definition of Setting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "Setting.h"

#include <QTimer>

#include "SettingItem.h"

Setting::Setting(Settings * Parent, const QString & Key, QVariant Default, Settings::Flags F)
	: QVariant(Default), settings(Parent), key(Key), flags(F), item(0)
{
	settings->construct(this);
}

Setting::~Setting()
{
	settings->destruct(this);
}

void Setting::setValue(const QVariant & value)
{
	if (*(QVariant *)this != value)
	{
		*(QVariant *)this = value;
		flags |= Settings::Dirty | Settings::Changed;
		settings->emitValueChanged(key);
	}
} // setValue()
