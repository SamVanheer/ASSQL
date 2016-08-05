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

	/*
	engine.RegisterObjectMethod(
		pszObjectName, "void BindNull(int iIndex)",
		asMETHOD( CASMySQLPreparedStatement, BindNull ), asCALL_THISCALL );
		*/

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, int iValue)",
		asMETHODPR( CASMySQLPreparedStatement, Bind, ( int, int ), void ), asCALL_THISCALL );

	/*
	engine.RegisterObjectMethod(
		pszObjectName, "void Bind64(int iIndex, int64 iValue)",
		asMETHODPR( CASMySQLPreparedStatement, Bind64, ( int, int64_t ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, double flValue)",
		asMETHODPR( CASMySQLPreparedStatement, Bind, ( int, double ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Bind(int iIndex, const string& in szString)",
		asMETHODPR( CASMySQLPreparedStatement, Bind, ( int, const std::string& ), void ), asCALL_THISCALL );
*/
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