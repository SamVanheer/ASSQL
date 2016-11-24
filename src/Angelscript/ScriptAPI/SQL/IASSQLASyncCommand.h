#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCCOMMAND_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCCOMMAND_H

#include "ASSQL.h"
#include "IASSQLASyncItem.h"

/**
*	A command that needs to be executed on a separate thread and possibly passed into a callback.
*/
class IASSQLASyncCommand : public IASSQLASyncItem
{
public:
	virtual SQLQueryResult::SQLQueryResult Execute() = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCCOMMAND_H