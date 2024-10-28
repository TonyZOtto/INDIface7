#pragma once

#include <QObject>

#include <QSize>

#include <VersionInfo.h>

class Objdet : public QObject
{
    Q_OBJECT
public: // types
    enum Class
    {
        $nullClass = 0,
        FaceFrontal,
        FaceProfileLeft,
        FaceProfileRight,
        EyeEither,
        EyeLeft,
        EyeRight,
        EyeBoth,
        $maxClass
    };
    Q_ENUM(Class)

public: // ctors
    explicit Objdet(QObject *parent = nullptr);

public slots:


public: // const
    VersionInfo cvVersion() const;
    QString className(const Class objcls);

public: // non-const

public: // static

    // ------------------ properties ---------------------
private:
    bool m_Enable;
    qreal m_Factor;
    int m_Flags;
    QSize m_MinSize;
    QSize m_MaxSize;
    int m_MaxDensity;
    int m_MaxDetectors;
    int m_MaxResults;
    bool m_ForceFind;
    int m_InputOverCrop;
    QString m_DetectorFile;
    Q_PROPERTY(bool Enable READ Enable WRITE Enable NOTIFY changed_Enable FINAL)
    Q_PROPERTY(qreal Factor READ Factor WRITE Factor NOTIFY changed_Factor FINAL)
    Q_PROPERTY(int Flags READ Flags WRITE Flags NOTIFY changed_Flags FINAL)
    Q_PROPERTY(QSize MinSize READ MinSize WRITE MinSize NOTIFY changed_MinSize FINAL)
    Q_PROPERTY(QSize MaxSize READ MaxSize WRITE MaxSize NOTIFY changed_MaxSize FINAL)
    Q_PROPERTY(int MaxDensity READ MaxDensity WRITE MaxDensity NOTIFY changed_MaxDensity FINAL)
    Q_PROPERTY(int MaxDetectors READ MaxDetectors WRITE MaxDetectors NOTIFY changed_MaxDetectors FINAL)
    Q_PROPERTY(int MaxResults READ MaxResults WRITE MaxResults NOTIFY changed_MaxResults FINAL)
    Q_PROPERTY(bool ForceFind READ ForceFind WRITE ForceFind NOTIFY changed_ForceFind FINAL)
    Q_PROPERTY(int InputOverCrop READ InputOverCrop WRITE InputOverCrop NOTIFY changed_InputOverCrop FINAL)
    Q_PROPERTY(QString DetectorFile READ DetectorFile WRITE DetectorFile NOTIFY changed_DetectorFile FINAL)

public:
    bool Enable() const;
    void Enable(bool new_Enable);
    qreal Factor() const;
    void Factor(qreal new_Factor);
    int Flags() const;
    void Flags(int new_Flags);
    QSize MinSize() const;
    void MinSize(const QSize &new_MinSize);
    QSize MaxSize() const;
    void MaxSize(const QSize &new_MaxSize);
    int MaxDensity() const;
    void MaxDensity(int new_MaxDensity);
    int MaxDetectors() const;
    void MaxDetectors(int new_MaxDetectors);
    int MaxResults() const;
    void MaxResults(int new_MaxResults);
    bool ForceFind() const;
    void ForceFind(bool new_ForceFind);
    int InputOverCrop() const;
    void InputOverCrop(int new_InputOverCrop);
    QString DetectorFile() const;
    void DetectorFile(const QString &new_DetectorFile);

signals:
    void changed_Enable();
    void changed_Factor();
    void changed_Flags();
    void changed_MinSize();
    void changed_MaxSize();
    void changed_MaxDensity();
    void changed_MaxDetectors();
    void changed_MaxResults();
    void changed_ForceFind();
    void changed_InputOverCrop();
    void changed_DetectorFile();
};

inline qreal Objdet::Factor() const
{
    return m_Factor;
}

inline void Objdet::Factor(qreal new_Factor)
{
    if (qFuzzyCompare(m_Factor, new_Factor))
        return;
    m_Factor = new_Factor;
    emit changed_Factor();
}

inline int Objdet::Flags() const
{
    return m_Flags;
}

inline void Objdet::Flags(int new_Flags)
{
    if (m_Flags == new_Flags)
        return;
    m_Flags = new_Flags;
    emit changed_Flags();
}

inline QSize Objdet::MinSize() const
{
    return m_MinSize;
}

inline void Objdet::MinSize(const QSize &new_MinSize)
{
    if (m_MinSize == new_MinSize)
        return;
    m_MinSize = new_MinSize;
    emit changed_MinSize();
}

inline QSize Objdet::MaxSize() const
{
    return m_MaxSize;
}

inline void Objdet::MaxSize(const QSize &new_MaxSize)
{
    if (m_MaxSize == new_MaxSize)
        return;
    m_MaxSize = new_MaxSize;
    emit changed_MaxSize();
}

inline int Objdet::MaxDensity() const
{
    return m_MaxDensity;
}

inline void Objdet::MaxDensity(int new_MaxDensity)
{
    if (m_MaxDensity == new_MaxDensity)
        return;
    m_MaxDensity = new_MaxDensity;
    emit changed_MaxDensity();
}

inline int Objdet::MaxDetectors() const
{
    return m_MaxDetectors;
}

inline void Objdet::MaxDetectors(int new_MaxDetectors)
{
    if (m_MaxDetectors == new_MaxDetectors)
        return;
    m_MaxDetectors = new_MaxDetectors;
    emit changed_MaxDetectors();
}

inline int Objdet::MaxResults() const
{
    return m_MaxResults;
}

inline void Objdet::MaxResults(int new_MaxResults)
{
    if (m_MaxResults == new_MaxResults)
        return;
    m_MaxResults = new_MaxResults;
    emit changed_MaxResults();
}

inline bool Objdet::ForceFind() const
{
    return m_ForceFind;
}

inline void Objdet::ForceFind(bool new_ForceFind)
{
    if (m_ForceFind == new_ForceFind)
        return;
    m_ForceFind = new_ForceFind;
    emit changed_ForceFind();
}

inline int Objdet::InputOverCrop() const
{
    return m_InputOverCrop;
}

inline void Objdet::InputOverCrop(int new_InputOverCrop)
{
    if (m_InputOverCrop == new_InputOverCrop)
        return;
    m_InputOverCrop = new_InputOverCrop;
    emit changed_InputOverCrop();
}

inline QString Objdet::DetectorFile() const
{
    return m_DetectorFile;
}

inline void Objdet::DetectorFile(const QString &new_DetectorFile)
{
    if (m_DetectorFile == new_DetectorFile)
        return;
    m_DetectorFile = new_DetectorFile;
    emit changed_DetectorFile();
}

inline bool Objdet::Enable() const
{
    return m_Enable;
}

inline void Objdet::Enable(bool new_Enable)
{
    if (m_Enable == new_Enable)
        return;
    m_Enable = new_Enable;
    emit changed_Enable();
}
