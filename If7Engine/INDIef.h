#ifndef INDIEF_H
#define INDIEF_H

#include <qglobal.h>

#ifdef INDIEF_LIB
# define  Q_DECL_EXPORT
#else
# define  Q_DECL_IMPORT
#endif


class  INDIef
{
public:
    INDIef();
    ~INDIef();

private:

};

#endif // INDIEF_H
