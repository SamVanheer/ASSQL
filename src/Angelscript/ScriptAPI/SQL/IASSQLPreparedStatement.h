#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLPREPAREDSTATEMENT_H

#include "IASSQLASyncItem.h"

class asIScriptFunction;

class IASSQLPreparedStatement : public IASSQLASyncItem
{
public:

	/**
	*	@return Whether this statement is valid.
	*/
	virtual bool IsValid() const = 0;

	/**
	*	Binds an integer argument.
	*/
	virtual void Bind( int iIndex, int iValue ) = 0;

	/**
	*	Binds a float argument.
	*/
	virtual void Bind( int iIndex, double flValue ) = 0;

	/**
	*	Executes the statement.
	*	@param pRowCallback Optional. Callback to invoke for each row.
	*	@param pCallback Optional. Callback to invoke after the statement has been completed.
	*	@return Whether the statement could be executed at this time.
	*/
	virtual bool ExecuteStatement( asIScriptFunction* pRowCallback = nullptr, asIScriptFunction* pCallback = nullptr ) = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLPREPAREDSTATEMENT_H