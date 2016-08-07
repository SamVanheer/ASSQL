#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_ASMYSQLDATETIME_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_ASMYSQLDATETIME_H

#include <mysql.h>

#include <Angelscript/ScriptAPI/CASDateTime.h>
#include <Angelscript/ScriptAPI/ASCTime.h>

/**
*	Converts a MYSQL_TIME to a time instance.
*/
CASTime CASTime_FromMySQLTime( const MYSQL_TIME& myTime );

/**
*	Converts a time instance to MYSQL_TIME.
*/
MYSQL_TIME CASTime_MySQLTime( const CASTime& time );

/**
*	Converts a MYSQL_TIME to a date time instance.
*/
CASDateTime CASDateTime_FromMySQLTime( const MYSQL_TIME& myTime );

/**
*	Converts a date time instance to MYSQL_TIME.
*/
MYSQL_TIME CASDateTime_MySQLTime( const CASDateTime& dateTime );

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_ASMYSQLDATETIME_H