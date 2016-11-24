#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "CASMySQLConnection.h"
#include "CASMySQLQuery.h"
#include "CASMySQLPreparedStatement.h"
#include "CASMySQLResultSet.h"

#include "ASMySQL.h"

static std::string MySQLFieldTypeToString( const enum_field_types type )
{
	switch( type )
	{
	case MYSQL_TYPE_DECIMAL:		return "DECIMAL";
	case MYSQL_TYPE_TINY:			return "TINY";
	case MYSQL_TYPE_SHORT:			return "SHORT";
	case MYSQL_TYPE_LONG:			return "LONG";
	case MYSQL_TYPE_FLOAT:			return "FLOAT";
	case MYSQL_TYPE_DOUBLE:			return "DOUBLE";
	case MYSQL_TYPE_NULL:			return "NULL";
	case MYSQL_TYPE_TIMESTAMP:		return "TIMESTAMP";
	case MYSQL_TYPE_LONGLONG:		return "LONGLONG";
	case MYSQL_TYPE_INT24:			return "INT24";
	case MYSQL_TYPE_DATE:			return "DATE";
	case MYSQL_TYPE_TIME:			return "TIME";
	case MYSQL_TYPE_DATETIME:		return "DATETIME";
	case MYSQL_TYPE_YEAR:			return "YEAR";
	case MYSQL_TYPE_NEWDATE:		return "NEWDATE";
	case MYSQL_TYPE_VARCHAR:		return "VARCHAR";
	case MYSQL_TYPE_BIT:			return "BIT";
	case MYSQL_TYPE_NEWDECIMAL:		return "NEWDECIMAL";
	case MYSQL_TYPE_ENUM:			return "ENUM";
	case MYSQL_TYPE_SET:			return "SET";
	case MYSQL_TYPE_TINY_BLOB:		return "TINY_BLOB";
	case MYSQL_TYPE_MEDIUM_BLOB:	return "MEDIUM_BLOB";
	case MYSQL_TYPE_LONG_BLOB:		return "LONG_BLOB";
	case MYSQL_TYPE_BLOB:			return "BLOB";
	case MYSQL_TYPE_VAR_STRING:		return "VAR_STRING";
	case MYSQL_TYPE_STRING:			return "STRING";
	case MYSQL_TYPE_GEOMETRY:		return "GEOMETRY";
	case MAX_NO_FIELD_TYPES:		return "MAX_NO_FIELD_TYPES";
	default:						return "UNKNOWN";
	}
}

static void RegisterScriptMySQLFieldType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLFieldType";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_DECIMAL", MYSQL_TYPE_DECIMAL );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_TINY", MYSQL_TYPE_TINY );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_SHORT", MYSQL_TYPE_SHORT );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_LONG", MYSQL_TYPE_LONG );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_FLOAT", MYSQL_TYPE_FLOAT );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_DOUBLE", MYSQL_TYPE_DOUBLE );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_NULL", MYSQL_TYPE_NULL );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_TIMESTAMP", MYSQL_TYPE_TIMESTAMP );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_LONGLONG", MYSQL_TYPE_LONGLONG );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_INT24", MYSQL_TYPE_INT24 );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_DATE", MYSQL_TYPE_DATE );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_TIME", MYSQL_TYPE_TIME );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_DATETIME", MYSQL_TYPE_DATETIME );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_YEAR", MYSQL_TYPE_YEAR );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_NEWDATE", MYSQL_TYPE_NEWDATE );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_VARCHAR", MYSQL_TYPE_VARCHAR );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_BIT", MYSQL_TYPE_BIT );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_NEWDECIMAL", MYSQL_TYPE_NEWDECIMAL );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_ENUM", MYSQL_TYPE_ENUM );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_SET", MYSQL_TYPE_SET );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_TINY_BLOB", MYSQL_TYPE_TINY_BLOB );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_MEDIUM_BLOB", MYSQL_TYPE_MEDIUM_BLOB );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_LONG_BLOB", MYSQL_TYPE_LONG_BLOB );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_BLOB", MYSQL_TYPE_BLOB );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_VAR_STRING", MYSQL_TYPE_VAR_STRING );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_STRING", MYSQL_TYPE_STRING );
	engine.RegisterEnumValue( pszObjectName, "MYSQL_TYPE_GEOMETRY", MYSQL_TYPE_GEOMETRY );
	engine.RegisterEnumValue( pszObjectName, "MAX_NO_FIELD_TYPES", MAX_NO_FIELD_TYPES );

	engine.RegisterGlobalFunction(
		AS_STRING_OBJNAME " MySQLFieldTypeToString(const MySQLFieldType type)",
		asFUNCTION( MySQLFieldTypeToString ), asCALL_CDECL );
}

static void RegisterScriptMySQLQuery( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLQuery";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASMySQLQuery>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsValid() const",
		asMETHOD( CASMySQLQuery, IsValid ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void MySQLQueryCallback(SQLQueryResult::SQLQueryResult result, MySQLQuery@ pQuery)" );
}

static void RegisterScriptMySQLResultSet( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MySQLResultSet";

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASMySQLResultSet>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "uint32 GetFieldCount() const",
		asMETHOD( CASMySQLResultSet, GetFieldCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Next()",
		asMETHOD( CASMySQLResultSet, Next ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "MySQLFieldType GetColumnType(const uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetColumnType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsNull(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, IsNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool GetBoolean(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetBoolean ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int8 GetSigned8(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetUnsigned8(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int16 GetSigned16(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetUnsigned16(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int32 GetSigned32(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint32 GetUnsigned32(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int64 GetSigned64(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint64 GetUnsigned64(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetUnsigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFloat(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetFloat ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "double GetDouble(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, AS_STRING_OBJNAME " GetString(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetString ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CDateTime GetDate(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetDate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CTime GetTime(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CDateTime GetDateTime(uint32 uiColumn) const",
		asMETHOD( CASMySQLResultSet, GetDateTime ), asCALL_THISCALL );
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
		pszObjectName, "uint32 GetParamCount() const",
		asMETHOD( CASMySQLPreparedStatement, GetParamCount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindNull(uint32 uiIndex)",
		asMETHOD( CASMySQLPreparedStatement, BindNull ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindBoolean(uint uiIndex, bool bValue)",
		asMETHOD( CASMySQLPreparedStatement, BindBoolean ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned8(uint uiIndex, int8 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned8(uint uiIndex, uint8 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned8 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned16(uint uiIndex, int16 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned16(uint uiIndex, uint16 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned16 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned32(uint uiIndex, int32 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned32(uint uiIndex, uint32 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned32 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindSigned64(uint uiIndex, int64 iValue)",
		asMETHOD( CASMySQLPreparedStatement, BindSigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindUnsigned64(uint uiIndex, uint64 uiValue)",
		asMETHOD( CASMySQLPreparedStatement, BindUnsigned64 ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindFloat(uint uiIndex, float flValue)",
		asMETHOD( CASMySQLPreparedStatement, BindFloat ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDouble(uint uiIndex, double flValue)",
		asMETHOD( CASMySQLPreparedStatement, BindDouble ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindString(uint uiIndex, const " AS_STRING_OBJNAME "& in szString)",
		asMETHOD( CASMySQLPreparedStatement, BindString ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDate(uint uiIndex, const CDateTime& in date)",
		asMETHOD( CASMySQLPreparedStatement, BindDate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindTime(uint uiIndex, const CTime& in time)",
		asMETHOD( CASMySQLPreparedStatement, BindTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void BindDateTime(uint uiIndex, const CDateTime& in dateTime)",
		asMETHOD( CASMySQLPreparedStatement, BindDateTime ), asCALL_THISCALL );

	engine.RegisterFuncdef( "void MySQLResultSetCallback(SQLQueryResult::SQLQueryResult result, MySQLResultSet@ pResultSet)" );
	engine.RegisterFuncdef( "void MySQLPreparedStatementCallback(SQLQueryResult::SQLQueryResult result, MySQLPreparedStatement@ pStatement)" );

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
		pszObjectName, "bool Query(const " AS_STRING_OBJNAME "& in szQuery, MySQLQueryCallback@ pCallback = null)",
		asMETHOD( CASMySQLConnection, Query ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "MySQLPreparedStatement@ CreatePreparedStatement(const " AS_STRING_OBJNAME "& in szStatement)",
		asMETHOD( CASMySQLConnection, CreatePreparedStatement ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, AS_STRING_OBJNAME " GetCharSet() const",
		asMETHOD( CASMySQLConnection, GetCharSet ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetCharSet(const " AS_STRING_OBJNAME "& in szCharSet)",
		asMETHOD( CASMySQLConnection, SetCharSet ), asCALL_THISCALL );
}

void RegisterScriptMySQL( asIScriptEngine& engine )
{
	RegisterScriptMySQLFieldType( engine );
	RegisterScriptMySQLQuery( engine );
	RegisterScriptMySQLResultSet( engine );
	RegisterScriptMySQLPreparedStatement( engine );
	RegisterScriptMySQLConnection( engine );
}