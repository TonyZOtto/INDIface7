#pragma once
#include <QString>
#include <QStringList>

class BaseIdBehavior;

class AbstractIdString
{
public:
    AbstractIdString(BaseIdBehavior * behavior,
                     QString string=QString());
    AbstractIdString(BaseIdBehavior * behavior,
                     const char * chars);
    bool isNull(void) const;
    bool operator == (const AbstractIdString & other);
    bool isSectioned(void) const;
    void set(const QString & string);
    void append(const QString & section);
    void prepend(const QString & section);
    QString toString() const;
    QString data(void) const { return toString(); }
    QString operator() (void) const { return toString(); }
    QString sortable(void) const;
    QString sectionSeparator(void) const;
    QString section(int index, int repeatQutoed=0) const;
    QStringList sectionList(int first, int last=-1) const;
    QString sections(int first, int last=-1) const;
    int sectionCount(void) const;
    friend bool operator == (const AbstractIdString &lhs, const AbstractIdString &rhs);

protected:

private:
    BaseIdBehavior * _behavior;
    QString _string;
};

