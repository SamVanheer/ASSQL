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
	, m_szHost( pszHost )
	, m_szUser( pszUser )
	, m_szPass( pszPassword )
	, m_szDatabase( pszDatabase )
	, m_uiPort( uiPort )
	, m_szUnixSocket( pszUnixSocket )
	, m_uiClientFlag( clientflag )
{
}

CASMySQLConnection::~CASMySQLConnection()
{
}

MYSQL* CASMySQLConnection::Open()
{
	MYSQL* pConnection = mysql_init( nullptr );

	//Enable multiple statements in a query to allow for consistent API behavior with SQLite.
	auto pResult = mysql_real_connect(
		pConnection,
		m_szHost.c_str(), m_szUser.c_str(), m_szPass.c_str(), m_szDatabase.c_str(),
		m_uiPort, m_szUnixSocket.c_str(), m_uiClientFlag | CLIENT_MULTI_STATEMENTS );

	if( !pResult )
	{
		GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLConnection::MySQLConnection: %s\n", mysql_error( pConnection ) );
		Close( pConnection );
		pConnection = nullptr;
	}

	return pConnection;
}

void CASMySQLConnection::Close( MYSQL* pConnection )
{
	if( pConnection )
	{
		mysql_close( pConnection );
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