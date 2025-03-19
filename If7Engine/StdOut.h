#pragma once

#include <QObject>

#include <QtDebug>
#include <QtMessageHandler>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextCursor>

class QFile;
class QTextEdit;
class QWidget;

class StdOut : public QObject
{
    Q_OBJECT
public:
    StdOut(QWidget * pCentralWidget=nullptr);
    ~StdOut();

public slots:
    void write(const QString &text, const bool endl=false);
    void write(QtMsgType qmt, const QMessageLogContext &ctx, const QString &msg);
    void write(const bool endl);
    void writeLine(const QString &line);
    void writeLines(const QStringList &lines);
    void writeLines(const QStringList &lines, const QString &prefix);
    void errorLine(const QString &msg, const QtMsgType qmt=QtCriticalMsg);

public: // const
    bool isFileOpen() const;

public: // non-const
    bool open(const QFileInfo &fi=QFileInfo());
    bool open(QtMessageHandler * pHandler);
    void close();

protected slots:
    void writeCache();

private:
    static QString errorPrefix(const QtMsgType qmt);

private:
    QTextEdit * mpView=nullptr;
    QTextCursor mCursor;
    QString mTextBuffer;
    QStringList mViewCache;
    QFile * mpTextFile=nullptr;
    QtMessageHandler * mpOldHandler=nullptr;
    QtMessageHandler * mpMessageHandler=nullptr;
    static QString mFileEndLine;
};
