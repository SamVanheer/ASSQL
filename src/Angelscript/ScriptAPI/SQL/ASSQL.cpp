#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

#include "IASSQLConnection.h"
#include "IASSQLQuery.h"

#include "ASSQL.h"

static void RegisterScriptSQLQuery( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLQuery";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<IASSQLQuery>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( IASSQLQuery, IsValid ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLQueryCallback(SQLQuery@ pQuery)" );
}

void RegisterScriptSQLConnection( asIScriptEngine& engine, const char* const pszObjectName )
{
	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<IASSQLConnection>( &engine, pszObjectName );

	engine.RegisterObjectMethod( 
		pszObjectName, "bool IsOpen() const", 
		asMETHOD( IASSQLConnection, IsOpen ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Close()",
		asMETHOD( IASSQLConnection, Close ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Query(const string& in szQuery, SQLQueryCallback@ pCallback = null)",
		asMETHOD( IASSQLConnection, Query ), asCALL_THISCALL );
}

void RegisterScriptSQL( asIScriptEngine& engine )
{
	RegisterScriptSQLQuery( engine );
	RegisterScriptSQLConnection( engine, "SQLConnection" );
}