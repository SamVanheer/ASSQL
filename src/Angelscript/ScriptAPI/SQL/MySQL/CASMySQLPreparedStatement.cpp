#include <cassert>
#include <cstring>

#include <angelscript.h>

#include "../CASSQLThreadPool.h"

#include "ASMySQLDateTime.h"

#include "CASMySQLBind.h"
#include "CASMySQLConnection.h"
#include "CASMySQLResultSet.h"

#include "CASMySQLPreparedStatement.h"

CASMySQLPreparedStatement::CASMySQLPreparedStatement( CASMySQLConnection* pConnection, const char* const pszStatement )
{
	assert( pConnection );
	assert( pszStatement );

	m_pConnection = pConnection;

	m_pConnection->AddRef();

	m_pStatement = mysql_stmt_init( m_pConnection->GetConnection() );

	if( m_pStatement )
	{
		if( mysql_stmt_prepare( m_pStatement, pszStatement, strlen( pszStatement ) ) == 0 )
		{
			const int iParamCount = GetParamCount();

			if( iParamCount > 0 )
			{
				m_pVariables = new CASMySQLBind[ iParamCount ];
				m_pBinds = new MYSQL_BIND[ iParamCount ];

				memset( m_pBinds, 0, sizeof( MYSQL_BIND ) * iParamCount );
			}
		}
		else
		{
			m_pConnection->GetLogFunction()( "MySQLPreparedStatement::MySQLPreparedStatement: %s\n", mysql_error( m_pConnection->GetConnection() ) );
		}
	}
	else
	{
		m_pConnection->GetLogFunction()( "MySQLPreparedStatement::MySQLPreparedStatement: %s\n", mysql_error( m_pConnection->GetConnection() ) );
	}
}

CASMySQLPreparedStatement::~CASMySQLPreparedStatement()
{
	//Delete variables first so they can query binds for types - Solokiller
	delete[] m_pVariables;
	delete[] m_pBinds;

	if( m_pStatement )
	{
		if( mysql_stmt_close( m_pStatement ) )
		{
			m_pConnection->GetLogFunction()( "MySQLPreparedStatement::~MySQLPreparedStatement%s\n", mysql_error( m_pConnection->GetConnection() ) );
		}
	}

	m_pConnection->Release();
}

void CASMySQLPreparedStatement::Execute()
{
	bool bSuccess = true;

	if( m_pBinds && mysql_stmt_bind_param( m_pStatement, m_pBinds ) )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: %s\n", mysql_error( m_pConnection->GetConnection() ) );

		bSuccess = false;
	}

	if( bSuccess && mysql_stmt_execute( m_pStatement ) )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: %s\n", mysql_error( m_pConnection->GetConnection() ) );
		
		bSuccess = false;
	}

	if( bSuccess )
	{
		auto pResultSet = new CASMySQLResultSet( this );

		if( pResultSet->IsValid() )
		{
			if( m_pCallback )
			{
				bSuccess = m_pConnection->GetThreadPool().GetThreadQueue().AddItem( pResultSet, m_pCallback );
			}
		}
		else
			bSuccess = false;

		pResultSet->Release();
	}

	if( m_pCallback )
	{
		m_pCallback->Release();

		m_pCallback = nullptr;
	}
}

bool CASMySQLPreparedStatement::IsValid() const
{
	return m_pStatement != nullptr;
}

int CASMySQLPreparedStatement::GetParamCount() const
{
	return mysql_stmt_param_count( m_pStatement );
}

void CASMySQLPreparedStatement::BindNull( int iIndex )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_NULL, &m_pBinds[ iIndex ] );
}

void CASMySQLPreparedStatement::BindBoolean( int iIndex, bool bValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_TINY, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_uiVal64 = bValue;
}

void CASMySQLPreparedStatement::BindSigned8( int iIndex, int8_t iValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_TINY, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned8( int iIndex, uint8_t uiValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_TINY, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_uiVal64 = uiValue;

	m_pBinds[ iIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned16( int iIndex, int16_t iValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_SHORT, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned16( int iIndex, uint16_t uiValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_SHORT, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_uiVal64 = uiValue;

	m_pBinds[ iIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned32( int iIndex, int32_t iValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_LONG, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned32( int iIndex, uint32_t uiValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_LONG, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_uiVal64 = uiValue;

	m_pBinds[ iIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned64( int iIndex, int64_t iValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_LONGLONG, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned64( int iIndex, uint64_t uiValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_LONGLONG, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_uiVal64 = uiValue;

	m_pBinds[ iIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindFloat( int iIndex, float flValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_FLOAT, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_flValue32[ 0 ] = flValue;
}

void CASMySQLPreparedStatement::BindDouble( int iIndex, double flValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_DOUBLE, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_flValue64 = flValue;
}

void CASMySQLPreparedStatement::BindString( int iIndex, const std::string& szString )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_STRING, &m_pBinds[ iIndex ], szString.data(), szString.length() );
}

void CASMySQLPreparedStatement::BindDate( int iIndex, const CASDateTime& date )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	auto& var = m_pVariables[ iIndex ];
	auto& bind = m_pBinds[ iIndex ];

	var.Set( MYSQL_TYPE_DATE, &bind );

	var.m_Time = CASDateTime_MySQLTime( date.GetDate() );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

void CASMySQLPreparedStatement::BindTime( int iIndex, const CASTime& time )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	auto& var = m_pVariables[ iIndex ];
	auto& bind = m_pBinds[ iIndex ];

	var.Set( MYSQL_TYPE_TIME, &bind );

	var.m_Time = CASTime_MySQLTime( time );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

void CASMySQLPreparedStatement::BindDateTime( int iIndex, const CASDateTime& dateTime )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	auto& var = m_pVariables[ iIndex ];
	auto& bind = m_pBinds[ iIndex ];

	var.Set( MYSQL_TYPE_DATETIME, &bind );

	var.m_Time = CASDateTime_MySQLTime( dateTime );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

#include <Angelscript/wrapper/ASCallable.h>

bool CASMySQLPreparedStatement::ExecuteStatement( asIScriptFunction* pResultSetCallback, asIScriptFunction* pCallback )
{
	bool bSuccess = false;

	if( m_pConnection->GetThreadPool().AddItem( this, pCallback ) )
	{
		m_pCallback = pResultSetCallback;
		bSuccess = true;
	}
	else
	{
		if( pResultSetCallback )
			pResultSetCallback->Release();
	}

	if( pCallback )
		pCallback->Release();

	return bSuccess;
}
