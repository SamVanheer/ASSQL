#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>
#include <Angelscript/util/CASBaseClass.h>

#include "../ASSQL.h"

#include "CASSQLiteConnection.h"
#include "CASSQLitePreparedStatement.h"

#include "ASSQLite.h"

static void RegisterScriptSQLiteRow( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteRow";

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

	engine.RegisterFuncdef( "void SQLiteRowCallback(SQLiteRow@ pRow)" );
}

static void RegisterScriptSQLitePreparedStatement( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLitePreparedStatement";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASSQLitePreparedStatement>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( CASSQLitePreparedStatement, IsValid ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, int iValue)",
		asMETHODPR( CASSQLitePreparedStatement, Bind, ( int, int ), void ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLitePreparedStatementCallback(SQLitePreparedStatement@ pStatement)" );

	engine.RegisterObjectMethod(
		pszObjectName, "void ExecuteStatement(SQLiteRowCallback@ pRowCallback = null, SQLitePreparedStatementCallback@ pCallback = null)",
		asMETHOD( CASSQLitePreparedStatement, ExecuteStatement ), asCALL_THISCALL );
}

static void RegisterScriptSQLiteConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteConnection";

	RegisterScriptSQLConnection( engine, pszObjectName );

	as::RegisterCasts<IASSQLConnection, CASSQLiteConnection>( engine, "SQLConnection", pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLitePreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( CASSQLiteConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptSQLite( asIScriptEngine& engine )
{
	RegisterScriptSQLiteRow( engine );
	RegisterScriptSQLitePreparedStatement( engine );
	RegisterScriptSQLiteConnection( engine );
}