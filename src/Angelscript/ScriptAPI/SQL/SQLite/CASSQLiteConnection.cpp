#include <cassert>

#include "CASSQLiteConnection.h"

CASSQLiteConnection::CASSQLiteConnection( const char* const pszFilename )
{
	assert( pszFilename );

	if( SQLITE_OK != sqlite3_open( pszFilename, &m_pConnection ) )
	{
		m_pConnection = nullptr;
	}
}

CASSQLiteConnection::~CASSQLiteConnection()
{
	Close();
}

bool CASSQLiteConnection::IsOpen() const
{
	return m_pConnection != nullptr;
}

void CASSQLiteConnection::Close()
{
	if( IsOpen() )
	{
		sqlite3_close( m_pConnection );
		m_pConnection = nullptr;
	}
}