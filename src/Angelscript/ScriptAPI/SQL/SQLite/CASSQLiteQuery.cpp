#include <cassert>
#include <cstring>

#include "CASSQLiteConnection.h"

#include "CASSQLiteQuery.h"

CASSQLiteQuery::CASSQLiteQuery( CASSQLiteConnection* pConnection, const char* const pszQuery )
{
	assert( pConnection );
	assert( pszQuery );

	m_pConnection = pConnection;

	m_pConnection->AddRef();

	m_szQuery = pszQuery;
}

CASSQLiteQuery::~CASSQLiteQuery()
{
	m_pConnection->Release();
	m_pConnection = nullptr;
}

SQLQueryResult::SQLQueryResult CASSQLiteQuery::Execute()
{
	char* pszError;
	sqlite3_exec( m_pConnection->GetConnection(), m_szQuery.c_str(), nullptr, nullptr, &pszError );

	if( pszError )
	{
		m_pConnection->GetLogFunction()( "Error: %s\n", pszError );
		sqlite3_free( pszError );
	}

	return pszError ? SQLQueryResult::FAILED : SQLQueryResult::SUCCESS;
}