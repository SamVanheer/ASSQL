#include "ASMySQLDateTime.h"

CASTime CASTime_FromMySQLTime( const MYSQL_TIME& myTime )
{
	return CASTime( myTime.hour, myTime.minute, myTime.second );
}

MYSQL_TIME CASTime_MySQLTime( const CASTime& time )
{
	MYSQL_TIME myTime;

	myTime.year = 0;
	myTime.month = 0;
	myTime.day = 0;
	myTime.hour = time.GetHour();
	myTime.minute = time.GetMinute();
	myTime.second = time.GetSecond();
	myTime.neg = false;
	myTime.second_part = 0;
	myTime.time_type = MYSQL_TIMESTAMP_TIME;

	return myTime;
}

CASDateTime CASDateTime_FromMySQLTime( const MYSQL_TIME& myTime )
{
	return CASDateTime( 
		myTime.year, myTime.month, myTime.day,
		myTime.hour, myTime.minute, myTime.second );
}

MYSQL_TIME CASDateTime_MySQLTime( const CASDateTime& dateTime )
{
	MYSQL_TIME myTime;

	myTime.year = dateTime.GetYear();
	myTime.month = dateTime.GetMonth();
	myTime.day = dateTime.GetDay();
	myTime.hour = dateTime.GetHour();
	myTime.minute = dateTime.GetMinute();
	myTime.second = dateTime.GetSecond();
	myTime.neg = false;
	myTime.second_part = 0;
	myTime.time_type = !myTime.hour && !myTime.minute && !myTime.second ? MYSQL_TIMESTAMP_DATE : MYSQL_TIMESTAMP_DATETIME;

	return myTime;
}