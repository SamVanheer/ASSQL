#include <cassert>
#include <cstring>

#include <angelscript.h>

#include "../CASSQLThreadPool.h"

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
	}
}

CASMySQLPreparedStatement::~CASMySQLPreparedStatement()
{
	//Delete variables first so they can query binds for types - Solokiller
	delete[] m_pVariables;
	delete[] m_pBinds;

	m_pConnection->Release();
}

void CASMySQLPreparedStatement::Execute()
{
	bool bSuccess = true;

	if( m_pBinds && mysql_stmt_bind_param( m_pStatement, m_pBinds ) )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( std::string( mysql_error( m_pConnection->GetConnection() ) ) + '\n' );

		bSuccess = false;
	}

	if( bSuccess && mysql_stmt_execute( m_pStatement ) )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( std::string( mysql_error( m_pConnection->GetConnection() ) ) + '\n' );
		
		bSuccess = false;
	}

	if( m_pCallback )
	{
		if( bSuccess )
		{
			auto pResultSet = new CASMySQLResultSet( this );

			bSuccess = m_pConnection->GetThreadPool().GetThreadQueue().AddItem( pResultSet, m_pCallback );

			pResultSet->Release();
		}

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
	m_pVariables[ iIndex ].m_flValue = flValue;
}

void CASMySQLPreparedStatement::BindDouble( int iIndex, double flValue )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_DOUBLE, &m_pBinds[ iIndex ] );
	m_pVariables[ iIndex ].m_dValue = flValue;
}

void CASMySQLPreparedStatement::BindText( int iIndex, const std::string& szText )
{
	if( iIndex < 0 || iIndex >= GetParamCount() )
		return;

	m_pVariables[ iIndex ].Set( MYSQL_TYPE_STRING, &m_pBinds[ iIndex ], szText.data(), szText.length() );
}

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
