#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_ASSQLDATATYPE_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_ASSQLDATATYPE_H

/**
*	Data types that are supported in SQL.
*/
enum ASSQLDataType
{
	SQL_TYPE_NULL = 0,
	SQL_TYPE_INTEGER,
	SQL_TYPE_FLOAT,
	SQL_TYPE_TEXT,
	SQL_TYPE_BLOB	//TODO: supported by MySQL? - Solokiller
};

/**
*	Converts an SQL data type to a string representation.
*/
const char* ASSQLDataTypeToString( const ASSQLDataType type );

/**
*	Converts a string to an SQL data type.
*	If the string is null or invalid, returns SQL_TYPE_NULL.
*/
ASSQLDataType ASStringToSQLDataType( const char* const pszString );

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_ASSQLDATATYPE_H