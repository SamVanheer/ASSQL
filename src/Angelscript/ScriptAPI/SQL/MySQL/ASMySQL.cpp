#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "../ASSQL.h"

#include "CASMySQLConnection.h"
#include "CASMySQLPreparedStatement.h"
#include "CASMySQLResultSet.h"

#include "ASMySQL.h"

static void RegisterScriptMySQLResultSet( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLResultSet";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASMySQLResultSet>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetFieldCount() const",
		asMETHOD( CASMySQLResultSet, GetFieldCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Next()",
		asMETHOD( CASMySQLResultSet, Next ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsNull(int iColumn) const",
		asMETHOD( CASMySQLResultSet, IsNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool GetBoolean(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetBoolean ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int8 GetSigned8(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetUnsigned8(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int16 GetSigned16(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetUnsigned16(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int32 GetSigned32(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint32 GetUnsigned32(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int64 GetSigned64(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint64 GetUnsigned64(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFloat(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetFloat ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "double GetDouble(int iColumn) const",
		asMETHOD( CASMySQLResultSet, GetDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetString(int iColumn ) const",
		asMETHOD( CASMySQLResultSet, GetString ), asCALL_THISCALL );
}

static void RegisterScriptMySQLPreparedStatement( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLPreparedStatement";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASMySQLPreparedStatement>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( CASMySQLPreparedStatement, IsValid ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindNull(int iIndex)",
		asMETHOD( CASMySQLPreparedStatement, BindNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindBoolean(int iIndex, bool bValue)",
		asMETHOD( CASMySQLPreparedStatement, BindBoolean ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned8(int iIndex, int8 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned8(int iIndex, uint8 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned16(int iIndex, int16 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned16(int iIndex, uint16 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned32(int iIndex, int32 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned32(int iIndex, uint32 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned64(int iIndex, int64 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned64(int iIndex, uint64 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindFloat(int iIndex, float flValue)",
		asMETHOD( CASMySQLPreparedStatement, BindFloat ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDouble(int iIndex, double flValue)",
		asMETHOD( CASMySQLPreparedStatement, BindDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindString(int iIndex, const string& in szString)",
		asMETHOD( CASMySQLPreparedStatement, BindString ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDate(int iIndex, const CDateTime& in date)",
		asMETHOD( CASMySQLPreparedStatement, BindDate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindTime(int iIndex, const CTime& in time)",
		asMETHOD( CASMySQLPreparedStatement, BindTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDateTime(int iIndex, const CDateTime& in dateTime)",
		asMETHOD( CASMySQLPreparedStatement, BindDateTime ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void MySQLResultSetCallback(MySQLResultSet@ pResultSet)" );
	engine.RegisterFuncdef( "void MySQLPreparedStatementCallback(MySQLPreparedStatement@ pStatement)" );

	engine.RegisterObjectMethod(
		pszObjectName, "void ExecuteStatement(MySQLResultSetCallback@ pResultSetCallback = null, MySQLPreparedStatementCallback@ pCallback = null)",
		asMETHOD( CASMySQLPreparedStatement, ExecuteStatement ), asCALL_THISCALL );
}

static void RegisterScriptMySQLConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLConnection";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASMySQLConnection>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Query(const string& in szQuery, SQLQueryCallback@ pCallback = null)",
		asMETHOD( CASMySQLConnection, Query ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "MySQLPreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( CASMySQLConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptMySQL( asIScriptEngine& engine )
{
	RegisterScriptMySQLResultSet( engine );
	RegisterScriptMySQLPreparedStatement( engine );
	RegisterScriptMySQLConnection( engine );
}