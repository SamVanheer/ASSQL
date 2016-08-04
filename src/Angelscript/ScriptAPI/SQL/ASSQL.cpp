#include <angelscript.h>

#include <Angelscript/util/CASBaseClass.h>

#include "IASSQLConnection.h"
#include "IASSQLPreparedStatement.h"
#include "IASSQLQuery.h"
#include "IASSQLRow.h"

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
		pszObjectName, "int GetColumnInt(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnInt ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetColumnDouble(const int iColumn) const",
		asMETHOD( IASSQLRow, GetColumnDouble ), asCALL_THISCALL );

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
		pszObjectName, "bool Query(const string& in szQuery, SQLQueryCallback@ pCallback = nullptr)",
		asMETHOD( IASSQLConnection, Query ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLPreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( IASSQLConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptSQL( asIScriptEngine& engine )
{
	RegisterScriptSQLQuery( engine );
	RegisterScriptSQLRow( engine );
	RegisterScriptSQLPreparedStatement( engine );
	RegisterScriptSQLConnection( engine );
}