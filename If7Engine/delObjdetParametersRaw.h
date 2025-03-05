#pragma once

#include <QObject>

#include <QSize>

#include <opencv2/objdetect.hpp>

class ObjdetParametersRaw : public QObject
{
    Q_OBJECT
public: // types
    enum Flag
    {
        $null = 0,
        Canny = cv::CASCADE_DO_CANNY_PRUNING,
        Scale = cv::CASCADE_SCALE_IMAGE,
        Biggest = cv::CASCADE_FIND_BIGGEST_OBJECT,
        Rough = cv::CASCADE_DO_ROUGH_SEARCH,
        ForceRaw = 0x8000,
    };
    Q_DECLARE_FLAGS(FlagSet, Flag)
    Q_FLAG(FlagSet)

public: // ctors
    explicit ObjdetParametersRaw(QObject *parent = nullptr);

public: // const
    cv::Size cvMinSize() const;
    cv::Size cvMaxSize() const;

public: // non-const
    void set(const Flag f, bool is=true);

public: // pointers


    // ------------------ properties ---------------------
public:
    qreal Factor() const;
    int Neighbors() const;
    int Flags() const;
    QSize MinSize() const;
    QSize MaxSize() const;
public slots:
    void Factor(qreal new_Factor);
    void reset_Factor();
    void Neighbors(int new_Neighbors);
    void reset_Neighbors();
    void Flags(int new_Flags);
    void reset_Flags();
    void MinSize(const QSize &new_MinSize);
    void reset_MinSize();
    void MaxSize(const QSize &new_MaxSize);
    void reset_MaxSize();
signals:
    void changed_Factor(qreal Factor);
    void changed_Neighbors(int Neighbors);
    void changed_Flags(int Flags);
    void changed_MinSize(QSize MinSize);
    void changed_MaxSize(QSize MaxSize);
private:
    qreal mFactor;
    int mNeighbors;
    int mFlags;
    QSize mMinSize;
    QSize mMaxSize;
    Q_PROPERTY(qreal Factor READ Factor WRITE Factor RESET reset_Factor NOTIFY changed_Factor FINAL)
    Q_PROPERTY(int Neighbors READ Neighbors WRITE Neighbors RESET reset_Neighbors NOTIFY changed_Neighbors FINAL)
    Q_PROPERTY(int Flags READ Flags WRITE Flags RESET reset_Flags NOTIFY changed_Flags FINAL)
    Q_PROPERTY(QSize MinSize READ MinSize WRITE MinSize RESET reset_MinSize NOTIFY changed_MinSize FINAL)
    Q_PROPERTY(QSize MaxSize READ MaxSize WRITE MaxSize RESET reset_MaxSize NOTIFY changed_MaxSize FINAL)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ObjdetParametersRaw::FlagSet)

inline qreal ObjdetParametersRaw::Factor() const
{
    return mFactor;
}

inline void ObjdetParametersRaw::Factor(qreal new_Factor)
{
    if (qFuzzyCompare(mFactor, new_Factor))
        return;
    mFactor = new_Factor;
    emit changed_Factor(mFactor);
}

inline void ObjdetParametersRaw::reset_Factor()
{
    Factor(1.1);
}

inline int ObjdetParametersRaw::Neighbors() const
{
    return mNeighbors;
}

inline void ObjdetParametersRaw::Neighbors(int new_Neighbors)
{
    if (mNeighbors == new_Neighbors)
        return;
    mNeighbors = new_Neighbors;
    emit changed_Neighbors(mNeighbors);
}

inline void ObjdetParametersRaw::reset_Neighbors()
{
    Neighbors(3);
}

inline int ObjdetParametersRaw::Flags() const
{
    return mFlags;
}

inline void ObjdetParametersRaw::Flags(int new_Flags)
{
    if (mFlags == new_Flags)
        return;
    mFlags = new_Flags;
    emit changed_Flags(mFlags);
}

inline void ObjdetParametersRaw::reset_Flags()
{
    Flags(0);
}

inline QSize ObjdetParametersRaw::MinSize() const
{
    return mMinSize;
}

inline void ObjdetParametersRaw::MinSize(const QSize &new_MinSize)
{
    if (mMinSize == new_MinSize)
        return;
    mMinSize = new_MinSize;
    emit changed_MinSize(mMinSize);
}

inline void ObjdetParametersRaw::reset_MinSize()
{
    MinSize(QSize());
}

inline QSize ObjdetParametersRaw::MaxSize() const
{
    return mMaxSize;
}

inline void ObjdetParametersRaw::MaxSize(const QSize &new_MaxSize)
{
    if (mMaxSize == new_MaxSize)
        return;
    mMaxSize = new_MaxSize;
    emit changed_MaxSize(mMaxSize);
}

inline void ObjdetParametersRaw::reset_MaxSize()
{
    MaxSize(QSize());
}


