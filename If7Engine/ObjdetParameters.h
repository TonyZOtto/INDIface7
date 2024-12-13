#pragma once

#include <QObject>

class ObjdetParameters
{
public:
    ObjdetParameters();

    // ------------------ properties ---------------------
private:

    bool m_Enable;
    QString m_DetectorsXml; // Catalog
    QString m_OutputDir;

};
