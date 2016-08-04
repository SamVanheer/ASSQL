#include <cassert>
#include <cstring>

#include <sqlite3.h>

#include "ASSQLiteDataType.h"

const char* ASSQLiteDataTypeToString( const int type )
{
	switch( type )
	{
	default:
		assert( false );

	case SQLITE_NULL:		return "NULL";
	case SQLITE_INTEGER:	return "INTEGER";
	case SQLITE_FLOAT:		return "FLOAT";
	case SQLITE_TEXT:		return "TEXT";
	case SQLITE_BLOB:		return "BLOB";
	}
}

int ASStringToSQLiteDataType( const char* const pszString )
{
	if( !pszString )
		return SQLITE_NULL;

	if( strcmp( "NULL", pszString ) == 0 )
		return SQLITE_NULL;
	else if( strcmp( "INTEGER", pszString ) == 0 )
		return SQLITE_INTEGER;
	else if( strcmp( "FLOAT", pszString ) == 0 )
		return SQLITE_FLOAT;
	else if( strcmp( "TEXT", pszString ) == 0 )
		return SQLITE_TEXT;
	else if( strcmp( "BLOB", pszString ) == 0 )
		return SQLITE_BLOB;

	return SQLITE_NULL;
}