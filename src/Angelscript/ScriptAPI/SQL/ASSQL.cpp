#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

#include "ASSQLDataType.h"

#include "IASSQLConnection.h"
#include "IASSQLQuery.h"

#include "ASSQL.h"

static std::string ASScriptSQLDataTypeToString( const ASSQLDataType type )
{
	return ASSQLDataTypeToString( type );
}

static ASSQLDataType ASScriptStringToSQLDataType( const std::string& szString )
{
	return ASStringToSQLDataType( szString.c_str() );
}

static void RegisterScriptSQLDataType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLDataType";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "SQL_TYPE_NULL", static_cast<int>( SQL_TYPE_NULL ) );
	engine.RegisterEnumValue( pszObjectName, "SQL_TYPE_INTEGER", static_cast<int>( SQL_TYPE_INTEGER ) );
	engine.RegisterEnumValue( pszObjectName, "SQL_TYPE_FLOAT", static_cast<int>( SQL_TYPE_FLOAT ) );
	engine.RegisterEnumValue( pszObjectName, "SQL_TYPE_TEXT", static_cast<int>( SQL_TYPE_TEXT ) );
	engine.RegisterEnumValue( pszObjectName, "SQL_TYPE_BLOB", static_cast<int>( SQL_TYPE_BLOB ) );

	engine.RegisterGlobalFunction( 
		"string SQLDataTypeToString(const SQLDataType type)",
		asFUNCTION( ASScriptSQLDataTypeToString ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"SQLDataType StringToSQLDataType(const string& in szString)",
		asFUNCTION( ASScriptStringToSQLDataType ), asCALL_CDECL );
}

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
	RegisterScriptSQLDataType( engine );
	RegisterScriptSQLQuery( engine );
	RegisterScriptSQLConnection( engine, "SQLConnection" );
}