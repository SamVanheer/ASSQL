#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H

#include "IASSQLASyncItem.h"

class IASSQLQuery : public IASSQLASyncItem
{
public:

	virtual bool IsValid() const = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H