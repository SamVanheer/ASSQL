#include <cassert>

#include "../CASSQLThreadPool.h"

#include "CASSQLiteQuery.h"

#include "CASSQLiteConnection.h"

CASSQLiteConnection::CASSQLiteConnection( const char* const pszFilename, CASSQLThreadPool& pool )
	: m_ThreadPool( pool )
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

bool CASSQLiteConnection::Query( const std::string& szQuery )
{
	auto pQuery = new CASSQLiteQuery( this, szQuery.c_str() );

	if( !pQuery->IsValid() )
	{
		delete pQuery;
		return false;
	}

	m_ThreadPool.AddItem( pQuery, nullptr );

	pQuery->Release();

	return true;
}