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
	MYSQL* pConn = m_pConnection->Open();

	if( !pConn )
	{
		//Open reports any errors.
		return;
	}

	const int iResult = mysql_query( pConn, m_szQuery.c_str() );

	if( iResult != 0 )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLQuery::Execute: %s\n", mysql_error( pConn ) );
	}
	else
	{
		//Ignore the results.
		int iResult;

		MYSQL_RES* pResultSet;

		do
		{
			if( pResultSet = mysql_store_result( pConn ) )
				mysql_free_result( pResultSet );

			iResult = mysql_next_result( pConn );
		}
		while( iResult == 0 );

		if( iResult > 0 )
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLQuery::Execute: %s\n", mysql_error( pConn ) );
	}

	m_pConnection->Close( pConn );
}

bool CASMySQLQuery::IsValid() const
{
	return true;
}