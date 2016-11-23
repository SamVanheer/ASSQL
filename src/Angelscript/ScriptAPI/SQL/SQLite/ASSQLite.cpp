#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>
#include <Angelscript/util/CASBaseClass.h>

#include "../ASSQL.h"

#include "ASSQLiteDataType.h"

#include "CASSQLiteConnection.h"
#include "CASSQLitePreparedStatement.h"
#include "CASSQLiteQuery.h"

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
		AS_STRING_OBJNAME " SQLiteDataTypeToString(const SQLiteDataType type)",
		asFUNCTION( ASScriptSQLiteDataTypeToString ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"SQLiteDataType StringToSQLiteDataType(const " AS_STRING_OBJNAME "& in szString)",
		asFUNCTION( ASScriptStringToSQLiteDataType ), asCALL_CDECL );
}

static void RegisterScriptSQLiteQuery( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteQuery";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASSQLiteQuery>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( CASSQLiteQuery, IsValid ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLiteQueryCallback(SQLiteQuery@ pQuery)" );
}

static void RegisterScriptSQLiteRow( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteRow";

	//Non-reference counted object passed into callbacks only. Don't keep references to this. - Solokiller
	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "uint32 GetRowIndex() const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetRowIndex ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint32 GetColumnCount() const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLiteDataType GetColumnType(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetColumnType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsColumnNull(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, IsColumnNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int32 GetSigned32(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int64 GetSigned64(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetDouble(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, AS_STRING_OBJNAME " GetString(const uint32 uiColumn) const",
		asMETHOD( CASSQLitePreparedStatement::CASSQLiteRow, GetString ), asCALL_THISCALL );

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
		pszObjectName, "void BindNull(uint32 uiIndex)",
		asMETHOD( CASSQLitePreparedStatement, BindNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned32(uint32 uiIndex, int32 iValue)",
		asMETHOD( CASSQLitePreparedStatement, BindSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned64(uint32 uiIndex, int64 iValue)",
		asMETHOD( CASSQLitePreparedStatement, BindSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDouble(uint32 uiIndex, double flValue)",
		asMETHOD( CASSQLitePreparedStatement, BindDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindString(uint32 uiIndex, const " AS_STRING_OBJNAME "& in szString)",
		asMETHOD( CASSQLitePreparedStatement, BindString ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void SQLitePreparedStatementCallback(SQLitePreparedStatement@ pStatement)" );

	engine.RegisterObjectMethod(
		pszObjectName, "void ExecuteStatement(SQLiteRowCallback@ pRowCallback = null, SQLitePreparedStatementCallback@ pCallback = null)",
		asMETHOD( CASSQLitePreparedStatement, ExecuteStatement ), asCALL_THISCALL );
}

static void RegisterScriptSQLiteConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SQLiteConnection";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASSQLiteConnection>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsOpen() const",
		asMETHOD( CASSQLiteConnection, IsOpen ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Close()",
		asMETHOD( CASSQLiteConnection, Close ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Query(const " AS_STRING_OBJNAME "& in szQuery, SQLiteQueryCallback@ pCallback = null)",
		asMETHOD( CASSQLiteConnection, Query ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SQLitePreparedStatement@ CreatePreparedStatement(const " AS_STRING_OBJNAME "& in szStatement)",
		asMETHOD( CASSQLiteConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptSQLite( asIScriptEngine& engine )
{
	RegisterScriptSQLiteQuery( engine );
	RegisterScriptSQLDataType( engine );
	RegisterScriptSQLiteRow( engine );
	RegisterScriptSQLitePreparedStatement( engine );
	RegisterScriptSQLiteConnection( engine );
}