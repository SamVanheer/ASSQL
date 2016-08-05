#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "../ASSQL.h"

#include "CASMySQLConnection.h"

#include "ASMySQL.h"

static void RegisterScriptMySQLConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLConnection";

	RegisterScriptSQLConnection( engine, pszObjectName );

	as::RegisterCasts<IASSQLConnection, CASMySQLConnection>( engine, "SQLConnection", pszObjectName );

	/*
	engine.RegisterObjectMethod(
		pszObjectName, "MySQLPreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( CASMySQLConnection, CreatePreparedStatement ), asCALL_THISCALL );
		*/
}

void RegisterScriptMySQL( asIScriptEngine& engine )
{
	RegisterScriptMySQLConnection( engine );
}