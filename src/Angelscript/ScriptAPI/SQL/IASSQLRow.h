#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H

#include "IASSQLASyncItem.h"

class IASSQLRow : public IASSQLASyncItem
{
public:

	/**
	*	@return The column count.
	*/
	virtual int GetColumnCount() const = 0;

	/**
	*	Gets a column integer.
	*/
	virtual int GetColumnInt( int iColumn ) const = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H