#ifndef EIGENFACESIMILARITYRESULTLIST_H
#define EIGENFACESIMILARITYRESULTLIST_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define  Q_DECL_EXPORT
#else
# define  Q_DECL_IMPORT
#endif

#include <QtCore/QList>
#include "EigenFaceSimilarityResult.h"

class  EigenFaceSimilarityResultList
        : public QList<EigenFaceSimilarityResult>
{
public:
    EigenFaceSimilarityResultList();
};

#endif // EIGENFACESIMILARITYRESULTLIST_H
