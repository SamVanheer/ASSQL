#include <cassert>
#include <string>

#include <angelscript.h>

#include "ASSQL.h"

static std::string SQLQueryResultToString( const SQLQueryResult::SQLQueryResult result )
{
	switch( result )
	{
	case SQLQueryResult::SUCCESS:	return "Success";
	case SQLQueryResult::FAILED:	return "Failed";
	}

	assert( false );

	return "Unknown Query Result";
}

static void RegisterScriptSQLQueryResult( asIScriptEngine& scriptEngine )
{
	const char* const pszObjectName = "SQLQueryResult";

	const std::string szOldNS = scriptEngine.GetDefaultNamespace();

	scriptEngine.SetDefaultNamespace( pszObjectName );

	scriptEngine.RegisterEnum( pszObjectName );

	scriptEngine.RegisterEnumValue( pszObjectName, "SUCCESS", SQLQueryResult::SUCCESS );
	scriptEngine.RegisterEnumValue( pszObjectName, "FAILED", SQLQueryResult::FAILED );

	scriptEngine.RegisterGlobalFunction(
		AS_STRING_OBJNAME " ToString(const SQLQueryResult result)",
		asFUNCTION( SQLQueryResultToString ), asCALL_CDECL );

	scriptEngine.SetDefaultNamespace( szOldNS.c_str() );
}

void RegisterScriptSQLCommon( asIScriptEngine& scriptEngine )
{
	RegisterScriptSQLQueryResult( scriptEngine );
}