#include <cassert>
#include <cstring>

#include "ASMySQLDateTime.h"

#include "CASMySQLBind.h"
#include "CASMySQLConnection.h"
#include "CASMySQLPreparedStatement.h"

#include "CASMySQLResultSet.h"

CASMySQLResultSet::CASMySQLResultSet( CASMySQLPreparedStatement* pStatement, MYSQL_STMT* pMyStatement )
{
	assert( pStatement );
	assert( pMyStatement );

	m_pStatement = pStatement;

	m_pStatement->AddRef();

	m_pMyStatement = pMyStatement;

	const int iMaxLength = 1;
	mysql_stmt_attr_set( m_pMyStatement, STMT_ATTR_UPDATE_MAX_LENGTH, &iMaxLength );

	if( mysql_stmt_store_result( m_pMyStatement ) != 0 )
	{
		pStatement->GetConnection()->GetLogFunction()( "MySQLResultSet::MySQLResultSet: %s\n", mysql_error( m_pMyStatement->mysql ) );

		Destruct();
	}
	else
	{
		m_pResultSet = mysql_stmt_result_metadata( m_pMyStatement );

		if( m_pResultSet )
		{
			m_pFields = mysql_fetch_fields( m_pResultSet );

			const int iFieldCount = GetFieldCount();

			if( iFieldCount > 0 )
			{
				m_pBinds = new MYSQL_BIND[ iFieldCount ];

				memset( m_pBinds, 0, sizeof( MYSQL_BIND ) * iFieldCount );

				m_pVariables = new CASMySQLBind[ iFieldCount ];

				for( int iIndex = 0; iIndex < iFieldCount; ++iIndex )
				{
					m_pVariables[ iIndex ].SetOutput( m_pFields[ iIndex ], &m_pBinds[ iIndex ] );
				}

				mysql_stmt_bind_result( m_pMyStatement, m_pBinds );
			}
		}
	}
}

CASMySQLResultSet::~CASMySQLResultSet()
{
	Destruct();

	m_pStatement->Release();
}

void CASMySQLResultSet::Destruct()
{
	if( m_pResultSet )
	{
		mysql_free_result( m_pResultSet );
		m_pResultSet = nullptr;
	}

	mysql_stmt_free_result( m_pMyStatement );

	if( m_pBinds )
	{
		delete[] m_pBinds;
		m_pBinds = nullptr;
	}

	if( m_pVariables )
	{
		delete[] m_pVariables;
		m_pVariables = nullptr;
	}
}

void CASMySQLResultSet::CallbackInvoked()
{
	m_pStatement->m_bHandledResultSet = true;
}

bool CASMySQLResultSet::IsValid() const
{
	return m_pResultSet != nullptr;
}

int CASMySQLResultSet::GetFieldCount() const
{
	return mysql_stmt_field_count( m_pMyStatement );
}

bool CASMySQLResultSet::Next()
{
	return mysql_stmt_fetch( m_pMyStatement ) == 0;
}

bool CASMySQLResultSet::IsNull( int iColumn ) const
{
	return m_pVariables[ iColumn ].m_bIsNull != 0;
}

bool CASMySQLResultSet::GetBoolean( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return m_pVariables[ iColumn ].m_uiVal64 != 0;
}

int8_t CASMySQLResultSet::GetSigned8( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<int8_t>( m_pVariables[ iColumn ].m_iVal64 );
}

uint8_t CASMySQLResultSet::GetUnsigned8( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<uint8_t>( m_pVariables[ iColumn ].m_uiVal64 );
}

int16_t CASMySQLResultSet::GetSigned16( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<int16_t>( m_pVariables[ iColumn ].m_iVal64 );
}

uint16_t CASMySQLResultSet::GetUnsigned16( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<uint16_t>( m_pVariables[ iColumn ].m_uiVal64 );
}

int32_t CASMySQLResultSet::GetSigned32( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<int32_t>( m_pVariables[ iColumn ].m_iVal64 );
}

uint32_t CASMySQLResultSet::GetUnsigned32( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return static_cast<uint32_t>( m_pVariables[ iColumn ].m_uiVal64 );
}

int64_t CASMySQLResultSet::GetSigned64( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return m_pVariables[ iColumn ].m_iVal64;
}

uint64_t CASMySQLResultSet::GetUnsigned64( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return m_pVariables[ iColumn ].m_uiVal64;
}

float CASMySQLResultSet::GetFloat( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return m_pVariables[ iColumn ].m_flValue32[ 0 ];
}

double CASMySQLResultSet::GetDouble( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return 0;

	return m_pVariables[ iColumn ].m_flValue64;
}

std::string CASMySQLResultSet::GetString( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return "";

	auto& buffer = m_pVariables[ iColumn ].m_Buffer;

	return !buffer.empty() ? std::string( buffer.begin(), buffer.end() ) : "";
}

CASDateTime CASMySQLResultSet::GetDate( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return CASDateTime();

	return CASDateTime_FromMySQLTime( m_pVariables[ iColumn ].m_Time );
}

CASTime CASMySQLResultSet::GetTime( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return CASDateTime();

	return CASTime_FromMySQLTime( m_pVariables[ iColumn ].m_Time );
}

CASDateTime CASMySQLResultSet::GetDateTime( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetFieldCount() )
		return CASDateTime();

	return CASDateTime_FromMySQLTime( m_pVariables[ iColumn ].m_Time );
}
