#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>
#include <Angelscript/util/CASBaseClass.h>

#include "../ASSQL.h"

#include "ASSQLiteDataType.h"

#include "CASSQLiteConnection.h"
#include "CASSQLitePreparedStatement.h"

#include "ASSQLite.h"

static std::string ASScriptSQLiteDataTypeToString( const int type )
{
	return ASSQLiteDataTypeToString( type );
}

static int ASScriptStringToSQLiteDataType( const std::string& szString )
{
	return ASStringToSQLiteDataType( szString.c_str() );
}

static void RegisterScriptSQLDataType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteDataType";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "SQLITE_NULL", static_cast<int>( SQLITE_NULL ) );
	engine.RegisterEnumValue( pszObjectName, "SQLITE_INTEGER", static_cast<int>( SQLITE_INTEGER ) );
	engine.RegisterEnumValue( pszObjectName, "SQLITE_FLOAT", static_cast<int>( SQLITE_FLOAT ) );
	engine.RegisterEnumValue( pszObjectName, "SQLITE_TEXT", static_cast<int>( SQLITE_TEXT ) );
	engine.RegisterEnumValue( pszObjectName, "SQLITE_BLOB", static_cast<int>( SQLITE_BLOB ) );

	engine.RegisterGlobalFunction(
		"string SQLiteDataTypeToString(const SQLiteDataType type)",
		asFUNCTION( ASScriptSQLiteDataTypeToString ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"SQLiteDataType StringToSQLiteDataType(const string& in szString)",
		asFUNCTION( ASScriptStringToSQLiteDataType ), asCALL_CDECL );
}

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
		pszObjectName, "SQLiteDataType GetColumnType(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsColumnNull(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, IsColumnNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnInt(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnInt ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColumnInt64(const int iColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnInt64 ), asCALL_THISCALL );

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
		pszObjectName, "void BindNull(int iIndex)",
		asMETHOD( CASSQLitePreparedStatement, BindNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned32(int iIndex, int32 iValue)",
		asMETHOD( CASSQLitePreparedStatement, BindSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned64(int iIndex, int64 iValue)",
		asMETHOD( CASSQLitePreparedStatement, BindSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDouble(int iIndex, double flValue)",
		asMETHOD( CASSQLitePreparedStatement, BindDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindString(int iIndex, const string& in szString)",
		asMETHOD( CASSQLitePreparedStatement, BindString ), asCALL_THISCALL );

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
	RegisterScriptSQLDataType( engine );
	RegisterScriptSQLiteRow( engine );
	RegisterScriptSQLitePreparedStatement( engine );
	RegisterScriptSQLiteConnection( engine );
}