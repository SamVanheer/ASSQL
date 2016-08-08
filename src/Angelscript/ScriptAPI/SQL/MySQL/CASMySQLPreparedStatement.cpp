#include <cassert>
#include <cstring>

#include <angelscript.h>

#include <Angelscript/wrapper/ASCallable.h>

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

	m_szStatement = pszStatement;
}

CASMySQLPreparedStatement::~CASMySQLPreparedStatement()
{
	m_pConnection->Release();
}

void CASMySQLPreparedStatement::Execute()
{
	assert( m_bExecuting );

	if( MYSQL* pConn = m_pConnection->Open() )
	{
		bool bSuccess = true;

		MYSQL_STMT* pStatement = mysql_stmt_init( pConn );

		if( pStatement )
		{
			if( mysql_stmt_prepare( pStatement, m_szStatement.c_str(), m_szStatement.length() ) != 0 )
			{
				m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: Error preparing statement: %s\n", mysql_error( pConn ) );

				bSuccess = false;
			}
			else
			{
				const size_t uiParamCount = static_cast<size_t>( mysql_stmt_param_count( pStatement ) );

				if( uiParamCount != m_Binds.size() )
				{
					m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( 
						"MySQLPreparedStatement::Execute: Incorrect number of parameters!\nExpected %u, got %u\n", uiParamCount, m_Binds.size() );
				
					bSuccess = false;
				}
			}
		}
		else
		{
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: Error creating statement: %s\n", mysql_error( pConn ) );

			bSuccess = false;
		}

		if( bSuccess && !m_Binds.empty() && mysql_stmt_bind_param( pStatement, m_Binds.data() ) )
		{
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: Error binding parameters: %s\n", mysql_error( pConn ) );

			bSuccess = false;
		}

		if( bSuccess && mysql_stmt_execute( pStatement ) )
		{
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: Error executing statement: %s\n", mysql_error( pConn ) );
		
			bSuccess = false;
		}

		if( bSuccess )
		{
			auto pResultSet = new CASMySQLResultSet( this, pStatement );

			if( pResultSet->IsValid() )
			{
				if( m_pCallback )
				{
					//Should be false here.
					assert( !m_bHandledResultSet );

					bSuccess = m_pConnection->GetThreadPool().GetThreadQueue().AddItem( pResultSet, m_pCallback );

					//Block until the set has been handled.
					while( !m_bHandledResultSet )
					{
						std::this_thread::yield();
					}

					//Reset it. - Solokiller
					m_bHandledResultSet = false;
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

		if( pStatement )
		{
			if( mysql_stmt_close( pStatement ) )
			{
				m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "MySQLPreparedStatement::Execute: Error closing statement: %s\n", mysql_error( pConn ) );
			}
		}

		m_pConnection->Close( pConn );
	}
	else
	{
		//Open reports any errors.
	}

	m_bExecuting = false;
}

bool CASMySQLPreparedStatement::IsValid() const
{
	return !m_szStatement.empty();
}

uint32_t CASMySQLPreparedStatement::GetParamCount() const
{
	return static_cast<uint32_t>( m_Binds.size() );
}

void CASMySQLPreparedStatement::BindNull( uint32_t uiIndex )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_NULL, &m_Binds[ uiIndex ] );
}

void CASMySQLPreparedStatement::BindBoolean( uint32_t uiIndex, bool bValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_TINY, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_uiVal64 = bValue;
}

void CASMySQLPreparedStatement::BindSigned8( uint32_t uiIndex, int8_t iValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_TINY, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned8( uint32_t uiIndex, uint8_t uiValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_TINY, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_uiVal64 = uiValue;

	m_Binds[ uiIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned16( uint32_t uiIndex, int16_t iValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_SHORT, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned16( uint32_t uiIndex, uint16_t uiValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_SHORT, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_uiVal64 = uiValue;

	m_Binds[ uiIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned32( uint32_t uiIndex, int32_t iValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_LONG, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned32( uint32_t uiIndex, uint32_t uiValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_LONG, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_uiVal64 = uiValue;

	m_Binds[ uiIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindSigned64( uint32_t uiIndex, int64_t iValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_LONGLONG, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_iVal64 = iValue;
}

void CASMySQLPreparedStatement::BindUnsigned64( uint32_t uiIndex, uint64_t uiValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_LONGLONG, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_uiVal64 = uiValue;

	m_Binds[ uiIndex ].is_unsigned = true;
}

void CASMySQLPreparedStatement::BindFloat( uint32_t uiIndex, float flValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_FLOAT, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_flValue32[ 0 ] = flValue;
}

void CASMySQLPreparedStatement::BindDouble( uint32_t uiIndex, double flValue )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_DOUBLE, &m_Binds[ uiIndex ] );
	m_Variables[ uiIndex ].m_flValue64 = flValue;
}

void CASMySQLPreparedStatement::BindString( uint32_t uiIndex, const std::string& szString )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	m_Variables[ uiIndex ].Set( MYSQL_TYPE_STRING, &m_Binds[ uiIndex ], szString.data(), szString.length() );
}

void CASMySQLPreparedStatement::BindDate( uint32_t uiIndex, const CASDateTime& date )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	auto& var = m_Variables[ uiIndex ];
	auto& bind = m_Binds[ uiIndex ];

	var.Set( MYSQL_TYPE_DATE, &bind );

	var.m_Time = CASDateTime_MySQLTime( date.GetDate() );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

void CASMySQLPreparedStatement::BindTime( uint32_t uiIndex, const CASTime& time )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	auto& var = m_Variables[ uiIndex ];
	auto& bind = m_Binds[ uiIndex ];

	var.Set( MYSQL_TYPE_TIME, &bind );

	var.m_Time = CASTime_MySQLTime( time );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

void CASMySQLPreparedStatement::BindDateTime( uint32_t uiIndex, const CASDateTime& dateTime )
{
	if( !CheckBindListSize( uiIndex ) )
		return;

	auto& var = m_Variables[ uiIndex ];
	auto& bind = m_Binds[ uiIndex ];

	var.Set( MYSQL_TYPE_DATETIME, &bind );

	var.m_Time = CASDateTime_MySQLTime( dateTime );

	bind.buffer = ( void* ) &var.m_Time;
	bind.buffer_length = sizeof( MYSQL_TIME );
}

bool CASMySQLPreparedStatement::ExecuteStatement( asIScriptFunction* pResultSetCallback, asIScriptFunction* pCallback )
{
	if( m_bExecuting )
	{
		m_pConnection->GetLogFunction()( "MySQLPreparedStatement::ExecuteStatement: Statement is already being executed!\n" );
		return false;
	}

	bool bSuccess = false;

	if( m_pConnection->GetThreadPool().AddItem( this, pCallback ) )
	{
		m_bExecuting = true;
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

bool CASMySQLPreparedStatement::CheckBindListSize( uint32_t uiIndex )
{
	const size_t uiOldSize = m_Binds.size();

	const size_t uiRequestedCount = uiIndex + 1;

	if( uiRequestedCount < uiOldSize )
		return true;

	//Have to keep the old versions around for a bit to copy everything over. - Solokiller
	std::vector<MYSQL_BIND> oldBinds = std::move( m_Binds );
	std::vector<CASMySQLBind> oldVars = std::move( m_Variables );

	m_Binds.resize( uiRequestedCount );
	m_Variables.resize( uiRequestedCount );

	//Zero out all binds.
	memset( m_Binds.data(), 0, sizeof( MYSQL_BIND ) * m_Binds.size() );

	//Copy over all data so the binds point to valid data.
	for( size_t uiVarIndex = 0; uiVarIndex < uiOldSize; ++uiVarIndex )
	{
		auto& oldVar = oldVars[ uiVarIndex ];
		auto& var = m_Variables[ uiVarIndex ];

		if( oldVar.m_pBind )
		{
			auto& oldBind = oldBinds[ uiVarIndex ];
			auto& bind = m_Binds[ uiVarIndex ];

			//Strings and blobs need this.
			const bool bUseBuffer = oldVar.m_Buffer.data() == oldBind.buffer;

			var.Set( oldBind.buffer_type,
					 &bind,
					 bUseBuffer ? oldVar.m_Buffer.data() : nullptr, 
					 bUseBuffer ? oldBind.buffer_length : 0 );

			//Just copy over the values. If it's not used, it's irrelevant.
			bind.is_unsigned = oldBind.is_unsigned;
			var.m_uiVal64 = oldVar.m_uiVal64;

			//It's a time value so copy over the time data.
			if( oldBind.buffer == &oldVar.m_Time )
			{
				var.m_Time = oldVar.m_Time;
			}
		}
	}

	return true;
}
