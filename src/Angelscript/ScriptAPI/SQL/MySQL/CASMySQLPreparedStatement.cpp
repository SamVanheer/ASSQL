#include <cassert>
#include <cstring>

#include <angelscript.h>

#include "../CASSQLThreadPool.h"

#include "CASMySQLConnection.h"

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
				m_pVariables = new Variable[ iParamCount ];
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
	if( m_pBinds && mysql_stmt_bind_param( m_pStatement, m_pBinds ) )
	{
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( std::string( mysql_error( m_pConnection->GetConnection() ) ) + '\n' );
		return;
	}

	if( mysql_stmt_execute( m_pStatement ) )
		m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( std::string( mysql_error( m_pConnection->GetConnection() ) ) + '\n' );
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

bool CASMySQLPreparedStatement::ExecuteStatement( asIScriptFunction* pCallback )
{
	bool bSuccess = false;

	if( m_pConnection->GetThreadPool().AddItem( this, pCallback ) )
	{
		bSuccess = true;
	}

	if( pCallback )
		pCallback->Release();

	return bSuccess;
}

CASMySQLPreparedStatement::Variable::~Variable()
{
	Clear();
}

void CASMySQLPreparedStatement::Variable::Set( enum_field_types type, MYSQL_BIND* pBind, const char* const pBuffer, const size_t uiLength )
{
	Clear();

	m_pBind = pBind;

	memset( m_pBind, 0, sizeof( MYSQL_BIND ) );

	m_pBind->buffer_type = type;
	m_pBind->buffer = &m_iVal64;
	m_pBind->length = &m_pBind->buffer_length;
	m_pBind->is_null = &m_bIsNull;
	m_pBind->error = &m_bError;

	m_pBind->buffer_length = 0;

	switch( type )
	{
	case MYSQL_TYPE_NULL:
		m_pBind->buffer_length = 1;
		break;

	case MYSQL_TYPE_BIT:
		m_pBind->buffer_length = 1;
		break;

	case MYSQL_TYPE_TINY:
		m_pBind->buffer_length = sizeof( int8_t );
		break;

	case MYSQL_TYPE_SHORT:
		m_pBind->buffer_length = sizeof( int16_t );
		break;

	case MYSQL_TYPE_LONG:
		m_pBind->buffer_length = sizeof( int32_t );
		break;

	case MYSQL_TYPE_LONGLONG:
		m_pBind->buffer_length = sizeof( int64_t );
		break;

	case MYSQL_TYPE_FLOAT:
		m_pBind->buffer_length = sizeof( float );
		break;

	case MYSQL_TYPE_DOUBLE:
		m_pBind->buffer_length = sizeof( double );
		break;

		//TODO

	default:
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
		{
			if( uiLength > 0 )
			{
				m_Buffer.resize( uiLength );

				m_pBind->buffer = m_Buffer.data();
				m_pBind->buffer_length = m_Buffer.size();

				if( pBuffer )
				{
					memcpy( m_Buffer.data(), pBuffer, uiLength );
				}
			}

			break;
		}
	}
}

void CASMySQLPreparedStatement::Variable::Clear()
{
	//TODO
	m_Buffer.clear();
	m_pBind = nullptr;
}