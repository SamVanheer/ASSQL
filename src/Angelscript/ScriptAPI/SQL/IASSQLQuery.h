#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H

#include "IASSQLASyncCommand.h"

class IASSQLQuery : public IASSQLASyncCommand
{
public:

	virtual bool IsValid() const = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLQUERY_H