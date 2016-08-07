#include <cassert>

#include "../CASSQLThreadPool.h"

#include "CASMySQLPreparedStatement.h"
#include "CASMySQLQuery.h"

#include "CASMySQLConnection.h"

CASMySQLConnection::CASMySQLConnection( CASSQLThreadPool& pool,
										const char* const pszHost, const char* const pszUser, const char* const pszPassword, 
										const char* const pszDatabase, const unsigned int uiPort, const char* const pszUnixSocket,
										unsigned long clientflag )
	: m_ThreadPool( pool )
{
	m_pConnection = mysql_init( nullptr );

	//Enable multiple statements in a query to allow for consistent API behavior with SQLite.
	auto pResult = mysql_real_connect( m_pConnection, pszHost, pszUser, pszPassword, pszDatabase, uiPort, pszUnixSocket, clientflag | CLIENT_MULTI_STATEMENTS );

	if( !pResult )
	{
		GetLogFunction()( "MySQLConnection::MySQLConnection: %s\n", mysql_error( m_pConnection ) );
		Close();
	}
}

CASMySQLConnection::~CASMySQLConnection()
{
	Close();
}

bool CASMySQLConnection::IsOpen() const
{
	return m_pConnection != nullptr;
}

void CASMySQLConnection::Close()
{
	if( m_pConnection )
	{
		mysql_close( m_pConnection );
		m_pConnection = nullptr;
	}
}

bool CASMySQLConnection::Query( const std::string& szQuery, asIScriptFunction* const pCallback )
{
	auto pQuery = new CASMySQLQuery( this, szQuery.c_str() );

	bool bSuccess = false;

	if( pQuery->IsValid() )
	{
		bSuccess = m_ThreadPool.AddItem( pQuery, pCallback );
	}

	if( pCallback )
		pCallback->Release();

	pQuery->Release();

	return bSuccess;
}

CASMySQLPreparedStatement* CASMySQLConnection::CreatePreparedStatement( const std::string& szStatement )
{
	auto pStatement = new CASMySQLPreparedStatement( this, szStatement.c_str() );

	if( pStatement->IsValid() )
		return pStatement;

	pStatement->Release();

	return nullptr;
}