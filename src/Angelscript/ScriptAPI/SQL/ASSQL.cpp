#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

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

void RegisterScriptSQL( asIScriptEngine& engine )
{
	RegisterScriptSQLQuery( engine );
}