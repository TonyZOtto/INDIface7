#include "DomValuesElement.h"

DomValuesElement::DomValuesElement(QDomElement *pDE)
    : pParentDE(pDE) {;}

DomValuesElement::DomValuesElement(const QString &aElementTag,
                       QDomElement *pDE)
    : pParentDE(pDE)
    , cmTag(aElementTag) {;}
