#include "ObjectHelper.h"

#include <QByteArray>
#include <QByteArrayList>
#include <QMetaEnum>
#include <QMetaObject>


ObjectHelper::ObjectHelper(const QObject *obj) : cmpObject(obj) {;}

const QMetaObject * ObjectHelper::metaObject() const
{
    const QMetaObject * result = nullptr;
    Q_CHECK_PTR(cmpObject);
    result = cmpObject->metaObject();
    Q_CHECK_PTR(result);
    //INFO << cmpObject->objectName() << result->className();
    return result;
}

const QMetaEnum ObjectHelper::metaEnum(const QString &enumName) const
{
    QMetaEnum result;
    const QMetaObject * pMetaObject = metaObject();
    const int cEnumIndex = pMetaObject->
                    indexOfEnumerator(QByteArray(enumName.toLocal8Bit()));
    if (cEnumIndex >= 0)
        result = pMetaObject->enumerator(cEnumIndex);
    //INFO << cmpObject->objectName() << cEnumIndex << result.enumName();
    return result;
}

QStringList ObjectHelper::namesOfEnums(const bool all) const
{
    QStringList result;
    const QMetaObject * pMetaObject = metaObject();
    const unsigned cEnumCount = pMetaObject->enumeratorCount();
    const signed cEnumBase = all ? 0 : pMetaObject->enumeratorOffset();
    for (unsigned ix = cEnumBase; ix < cEnumCount; ++ix)
        result << pMetaObject->enumerator(ix).name();
    return result;
}

QStringList ObjectHelper::keysInEnum(const QString &enumName) const
{
    QStringList result;
#ifdef QT_DEBUG
    Q_ASSERT(namesOfEnums().contains(enumName));
#endif
    const QMetaEnum cMetaEnum = metaEnum(enumName);
    if (cMetaEnum.isValid())
    {
        const unsigned cKeyCount = cMetaEnum.keyCount();
        for (unsigned ix = 0; ix < cKeyCount; ++ix)
            result << cMetaEnum.key(ix);
    }
    return result;
}

bool ObjectHelper::enumIsFlags(const QString &enumName) const
{
    bool result = false;
    const QMetaEnum cMetaEnum = metaEnum(enumName);
    if (cMetaEnum.isValid() && cMetaEnum.isFlag()) result = true;
    return result;
}

QString ObjectHelper::enumKey(const QString &enumName, const int value) const
{
    QString result;
    const QMetaEnum cMetaEnum = metaEnum(enumName);
    if (cMetaEnum.isValid())
        result = cMetaEnum.valueToKey(value);
    //INFO << enumName << value << result;
    return result;
}

int ObjectHelper::enumValue(const QString &enumName, const QString &key) const
{
    int result = -2;
    const QMetaEnum cMetaEnum = metaEnum(enumName);
    if (cMetaEnum.isValid())
        result = cMetaEnum.keyToValue(QByteArray(key.toLocal8Bit()));
    //INFO << enumName << key << result;
    return result;
}

QStringList ObjectHelper::flagKeys(const QString &enumName, int flags, const bool isSet) const
{
    qInfo() << Q_FUNC_INFO << enumName << Qt::hex << flags << isSet;
    QStringList result;
    const QMetaEnum cMetaEnum = metaEnum(enumName);
    qDebug() << cMetaEnum.enumName()
             << (cMetaEnum.isFlag() ? "isFlag" : "NOTflag")
             << (cMetaEnum.isValid() ? "isValid" : "NOTvalid");
    if (cMetaEnum.isValid())
    {
        if ( ! isSet) flags = ~ flags;
        QByteArray tKeys = cMetaEnum.valueToKeys(flags);
        QByteArrayList tKeyList = tKeys.split('|');
        foreach (QByteArray tBA, tKeyList)
            result << QString(tBA);
    }
#ifdef QT_DEBUG
    else
    {
        const QMetaType cMetaType = cMetaEnum.metaType();
        qWarning() << "Type" << cMetaType.id() << cMetaType.name() << cMetaType.flags();
    }
    qDebug() << Q_FUNC_INFO << "result" << result;
#endif
    return result;
}

