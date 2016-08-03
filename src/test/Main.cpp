#include "sqlite3.h"


int main( int iArgc, char* pszArgV[] )
{
	sqlite3* pConnection;

	if( SQLITE_OK == sqlite3_open( "file.sqlite", &pConnection ) )
	{
		sqlite3_close( pConnection );
	}

	return 0;
}