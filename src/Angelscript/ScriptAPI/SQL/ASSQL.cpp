#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

#include "ASSQLDataType.h"

#include "IASSQLConnection.h"
#include "IASSQLPreparedStatement.h"
#include "IASSQLQuery.h"
#include "IASSQLRow.h"

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

static void RegisterScriptSQLRow( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLRow";

	//Non-reference counted object passed into callbacks only. Don't keep references to this. - Solokiller
	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetRowIndex() const",
		asMETHOD( IASSQLRow, GetRowIndex ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnCount() const",
		asMETHOD( IASSQLRow, GetColumnCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLDataType GetColumnType(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsColumnNull(const int iColumn) const",
		asMETHOD( IASSQLRow, IsColumnNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnInt(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnInt ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetColumnDouble(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetColumnString(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnString ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLRowCallback(SQLRow@ pRow)" );
}

static void RegisterScriptSQLPreparedStatement( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLPreparedStatement";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<IASSQLPreparedStatement>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( IASSQLPreparedStatement, IsValid ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, int iValue)",
		asMETHODPR( IASSQLPreparedStatement, Bind, ( int, int ), void ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLPreparedStatementCallback(SQLPreparedStatement@ pStatement)" );

	engine.RegisterObjectMethod(
		pszObjectName, "void ExecuteStatement(SQLRowCallback@ pRowCallback = null, SQLPreparedStatementCallback@ pCallback = null)",
		asMETHOD( IASSQLPreparedStatement, ExecuteStatement ), asCALL_THISCALL );
}

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

	engine.RegisterObjectMethod(
		pszObjectName, "bool Query(const string& in szQuery, SQLQueryCallback@ pCallback = null)",
		asMETHOD( IASSQLConnection, Query ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLPreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( IASSQLConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptSQL( asIScriptEngine& engine )
{
	RegisterScriptSQLDataType( engine );
	RegisterScriptSQLQuery( engine );
	RegisterScriptSQLRow( engine );
	RegisterScriptSQLPreparedStatement( engine );
	RegisterScriptSQLConnection( engine );
}