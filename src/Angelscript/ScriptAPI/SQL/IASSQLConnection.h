#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H

#include <string>

class asIScriptFunction;

class IASSQLPreparedStatement;

/**
*	Represents an SQL connection.
*/
class IASSQLConnection
{
public:
	virtual void AddRef() const = 0;
	virtual void Release() const = 0;

	/**
	*	@return Whether the connection is open.
	*/
	virtual bool IsOpen() const = 0;

	/**
	*	If the connection is open, closes the connection.
	*/
	virtual void Close() = 0;

	/**
	*	Performs a simple query.
	*	@param szQuery Query to perform.
	*	@param pCallback Optional. Callback to invoke.
	*	@return Whether the query is well formed.
	*/
	virtual bool Query( const std::string& szQuery, asIScriptFunction* const pCallback = nullptr ) = 0;

	/**
	*	Creates a prepared statement.
	*	@param szStatement Statement.
	*	@return if the statement is valid, returns the statement. Otherwise, returns null.
	*/
	virtual IASSQLPreparedStatement* CreatePreparedStatement( const std::string& szStatement ) = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H