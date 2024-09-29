#pragma once

#include <QFile>
#include <QList>
#include <QPair>
#include <QTextStream>

#include "VariableId.h"
#include "VariableGroup.h"

class CsvWriter
{
public:
    CsvWriter(QIODevice * ioDevice);
    CsvWriter(const QString & fileName);
    bool isOpen(void) const;
    void add(VariableGroup * group,
             VariableIdList ids=VariableIdList());
    void header(void);
    void writeLine(void);
    void flush(void);
    void close(void);
    QString errorString(void) const;

private:
    typedef QPair<VariableId, VariableGroup *> VariablePair;
    QFile _file;
    QIODevice * _ioDevice;
    QTextStream _textStream;
    QList<VariablePair> _list;
    int csvRows;
};


