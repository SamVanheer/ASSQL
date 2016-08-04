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
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetRowIndex ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnCount() const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLDataType GetColumnType(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsColumnNull(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, IsColumnNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnInt(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnInt ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetColumnDouble(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetColumnText(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnText ), asCALL_THISCALL );

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

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, double flValue)",
		asMETHODPR( CASSQLitePreparedStatement, Bind, ( int, double ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, const string& in szString)",
		asMETHODPR( CASSQLitePreparedStatement, Bind, ( int, const std::string& ), void ), asCALL_THISCALL );

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