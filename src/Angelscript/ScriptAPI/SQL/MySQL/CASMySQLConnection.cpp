#include <iostream>

#include "CASMySQLConnection.h"

CASMySQLConnection::CASMySQLConnection( CASSQLThreadPool& pool, 
										const char* const pszHost, const char* const pszUser, const char* const pszPassword, 
										const char* const pszDatabase, const unsigned int uiPort, const char* const pszUnixSocket,
										unsigned long clientflag )
	: m_ThreadPool( pool )
{
	m_pConnection = mysql_init( nullptr );

	auto pResult = mysql_real_connect( m_pConnection, pszHost, pszUser, pszPassword, pszDatabase, uiPort, pszUnixSocket, clientflag );

	if( !pResult )
	{
		std::cout << mysql_error( m_pConnection ) << std::endl;
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
	if( pCallback )
		pCallback->Release();

	return false;
}

IASSQLPreparedStatement* CASMySQLConnection::CreatePreparedStatement( const std::string& szStatement )
{
	return nullptr;
}