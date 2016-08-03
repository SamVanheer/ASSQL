#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

#include "IASSQLConnection.h"

#include "ASSQL.h"

static void RegisterScriptSQLConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLConnection";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<IASSQLConnection>( &engine, pszObjectName );

	engine.RegisterObjectMethod( 
		pszObjectName, "bool IsOpen() const", 
		asMETHOD( IASSQLConnection, IsOpen ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Close()",
		asMETHOD( IASSQLConnection, Close ), asCALL_THISCALL );
}

void RegisterScriptSQL( asIScriptEngine& engine )
{
	RegisterScriptSQLConnection( engine );
}