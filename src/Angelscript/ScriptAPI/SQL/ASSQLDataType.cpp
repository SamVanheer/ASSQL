#include <cassert>
#include <cstring>

#include "ASSQLDataType.h"

const char* ASSQLDataTypeToString( const ASSQLDataType type )
{
	switch( type )
	{
	default:
		assert( false );

	case SQL_TYPE_NULL:		return "NULL";
	case SQL_TYPE_INTEGER:	return "INTEGER";
	case SQL_TYPE_FLOAT:	return "FLOAT";
	case SQL_TYPE_TEXT:		return "TEXT";
	case SQL_TYPE_BLOB:		return "BLOB";
	}
}

ASSQLDataType ASStringToSQLDataType( const char* const pszString )
{
	if( !pszString )
		return SQL_TYPE_NULL;

	if( strcmp( "NULL", pszString ) == 0 )
		return SQL_TYPE_NULL;
	else if( strcmp( "INTEGER", pszString ) == 0 )
		return SQL_TYPE_INTEGER;
	else if( strcmp( "FLOAT", pszString ) == 0 )
		return SQL_TYPE_FLOAT;
	else if( strcmp( "TEXT", pszString ) == 0 )
		return SQL_TYPE_TEXT;
	else if( strcmp( "BLOB", pszString ) == 0 )
		return SQL_TYPE_BLOB;

	return SQL_TYPE_NULL;
}