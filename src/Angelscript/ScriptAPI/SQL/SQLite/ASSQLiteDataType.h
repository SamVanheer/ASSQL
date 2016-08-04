#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_ASSQLITEDATATYPE_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_ASSQLITEDATATYPE_H

/**
*	Converts an SQL data type to a string representation.
*/
const char* ASSQLiteDataTypeToString( const int type );

/**
*	Converts a string to an SQL data type.
*	If the string is null or invalid, returns SQLITE_NULL.
*/
int ASStringToSQLiteDataType( const char* const pszString );

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_ASSQLITEDATATYPE_H