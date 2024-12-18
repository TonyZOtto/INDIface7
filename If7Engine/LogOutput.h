#pragma once

#include <QObject>

#include <QtDebug>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextCursor>

class QFile;
class QTextEdit;
class QWidget;

class LogOutput : public QObject
{
    Q_OBJECT
public:
    LogOutput(QWidget * pCentralWidget=nullptr);
    ~LogOutput();

public slots:
    void write(const QString &text, const bool endl=false);
    void write(const bool endl);
    void writeLine(const QString &line);
    void writeLines(const QStringList &lines);
    void writeLines(const QStringList &lines, const QString &prefix);
    void errorLine(const QString &msg, const QtMsgType qmt=QtCriticalMsg);

public: // const
    bool isFileOpen() const;

public: // non-const
    bool open(const QFileInfo &fi=QFileInfo());
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
    QFile * mpFile=nullptr;
    static QString mFileEndLine;
};
