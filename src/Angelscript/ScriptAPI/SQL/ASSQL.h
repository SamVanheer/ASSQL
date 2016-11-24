#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H

class asIScriptEngine;

/**
*	Log function pointer to use in SQL code.
*/
using ASSQLLogFunction = void ( * )( const char* const pszFormat, ... );

namespace SQLQueryResult
{
/**
*	Query execution results.
*/
enum SQLQueryResult
{
	/**
	*	Query executed successfully.
	*/
	SUCCESS = 0,

	/**
	*	Query failed for unknown reason.
	*/
	FAILED,
};
}

/**
*	Registers common SQL types & functions.
*	@param scriptEngine Script engine.
*/
void RegisterScriptSQLCommon( asIScriptEngine& scriptEngine );

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H