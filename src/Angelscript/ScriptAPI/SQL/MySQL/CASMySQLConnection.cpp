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
	, m_iTimeout( DEFAULT_TIMEOUT )
{
}

CASMySQLConnection::~CASMySQLConnection()
{
}

MYSQL* CASMySQLConnection::Open()
{
	//Data is immutable by the main thread during this operation.
	std::lock_guard<std::mutex> lock( m_DataMutex );

	MYSQL* pConnection = mysql_init( nullptr );

	const int iTimeout = m_iTimeout;

	mysql_options( pConnection, MYSQL_OPT_CONNECT_TIMEOUT, &iTimeout );
	mysql_options( pConnection, MYSQL_OPT_READ_TIMEOUT, &iTimeout );
	mysql_options( pConnection, MYSQL_OPT_WRITE_TIMEOUT, &iTimeout );

	//Automatically reconnect to the server.
	const my_bool my_true = true;
	mysql_options( pConnection, MYSQL_OPT_RECONNECT, &my_true );

	//Enable multiple statements in a query to allow for consistent API behavior with SQLite.
	auto pResult = mysql_real_connect(
		pConnection,
		m_szHost.c_str(), m_szUser.c_str(), m_szPass.c_str(), m_szDatabase.c_str(),
		m_uiPort, m_szUnixSocket.c_str(), m_uiClientFlag | CLIENT_MULTI_STATEMENTS );

	if( pResult )
	{
		if( !m_szCharSet.empty() )
			mysql_set_character_set( pConnection, m_szCharSet.c_str() );
	}
	else
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

std::string CASMySQLConnection::GetCharSet() const
{
	std::lock_guard<std::mutex> lock( m_DataMutex );

	return m_szCharSet;
}

void CASMySQLConnection::SetCharSet( const std::string& szCharSet )
{
	std::lock_guard<std::mutex> lock( m_DataMutex );

	m_szCharSet = szCharSet;
}
