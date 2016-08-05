#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "../ASSQL.h"

#include "CASMySQLConnection.h"
#include "CASMySQLPreparedStatement.h"

#include "ASMySQL.h"

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
		pszObjectName, "void BindText(int iIndex, const string& in szString)",
		asMETHOD( CASMySQLPreparedStatement, BindText ), asCALL_THISCALL );

	//TODO result set - Solokiller
	engine.RegisterFuncdef( "void MySQLPreparedStatementCallback(MySQLPreparedStatement@ pStatement)" );

	engine.RegisterObjectMethod(
		pszObjectName, "void ExecuteStatement(MySQLPreparedStatementCallback@ pCallback = null)",
		asMETHOD( CASMySQLPreparedStatement, ExecuteStatement ), asCALL_THISCALL );
}

static void RegisterScriptMySQLConnection( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLConnection";

	RegisterScriptSQLConnection( engine, pszObjectName );

	as::RegisterCasts<IASSQLConnection, CASMySQLConnection>( engine, "SQLConnection", pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "MySQLPreparedStatement@ CreatePreparedStatement(const string& in szStatement)",
		asMETHOD( CASMySQLConnection, CreatePreparedStatement ), asCALL_THISCALL );
}

void RegisterScriptMySQL( asIScriptEngine& engine )
{
	RegisterScriptMySQLPreparedStatement( engine );
	RegisterScriptMySQLConnection( engine );
}