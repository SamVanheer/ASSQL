#include <cassert>
#include <cstring>
#include <iostream>

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

void CASSQLiteQuery::Execute()
{
	char* pszError;
	sqlite3_exec( m_pConnection->GetConnection(), m_szQuery.c_str(), nullptr, nullptr, &pszError );

	if( pszError )
	{
		std::cout << "Error: " << pszError << std::endl;
		sqlite3_free( pszError );
	}
}