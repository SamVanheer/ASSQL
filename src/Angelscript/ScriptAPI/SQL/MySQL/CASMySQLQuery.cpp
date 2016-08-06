#include <cassert>

#include "../CASSQLThreadPool.h"

#include "CASMySQLConnection.h"

#include "CASMySQLQuery.h"

CASMySQLQuery::CASMySQLQuery( CASMySQLConnection* pConnection, const char* const pszQuery )
	: m_pConnection( pConnection )
{
	assert( pConnection );
	assert( pszQuery );

	m_pConnection->AddRef();

	m_szQuery = pszQuery;
}

CASMySQLQuery::~CASMySQLQuery()
{
	m_pConnection->Release();
}

void CASMySQLQuery::Execute()
{
	if( !m_pConnection->IsOpen() )
		return;

	const int iResult = mysql_query( m_pConnection->GetConnection(), m_szQuery.c_str() );

	if( iResult != 0 )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "%s\n", mysql_error( m_pConnection->GetConnection() ) );
	}
	else
	{
		//Ignore the results.
		int iResult;

		MYSQL_RES* pResultSet;

		do
		{
			if( pResultSet = mysql_store_result( m_pConnection->GetConnection() ) )
				mysql_free_result( pResultSet );

			iResult = mysql_next_result( m_pConnection->GetConnection() );
		}
		while( iResult == 0 );

		if( iResult > 0 )
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "%s\n", mysql_error( m_pConnection->GetConnection() ) );
	}
}

bool CASMySQLQuery::IsValid() const
{
	return true;
}