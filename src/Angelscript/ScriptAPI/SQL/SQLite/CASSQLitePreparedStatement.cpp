#include <cassert>
#include <thread>

#include "../CASSQLThreadPool.h"
#include "../CASSQLThreadQueue.h"

#include"CASSQLiteConnection.h"

#include "CASSQLitePreparedStatement.h"

CASSQLitePreparedStatement::CASSQLitePreparedStatement( CASSQLiteConnection* pConnection, const char* const pszStatement )
{
	assert( pConnection );
	assert( pszStatement );

	m_pConnection = pConnection;

	m_pConnection->AddRef();

	if( SQLITE_OK != sqlite3_prepare_v2( m_pConnection->GetConnection(), pszStatement, strlen( pszStatement ) + 1, &m_pStatement, nullptr ) )
	{
		m_pStatement = nullptr;
	}
}

CASSQLitePreparedStatement::~CASSQLitePreparedStatement()
{
	if( m_pRowCallback )
	{
		m_pConnection->GetLogFunction()( 
			"SQLite: Row callback is still null in prepared statement destructor!\nStatement SQL: %s\n", 
			m_pStatement ? sqlite3_sql( m_pStatement ) : "Unknown" );
		m_pRowCallback->Release();
		m_pRowCallback = nullptr;
	}

	if( m_pStatement )
	{
		sqlite3_finalize( m_pStatement );
		m_pStatement = nullptr;
	}

	m_pConnection->Release();
}

void CASSQLitePreparedStatement::Execute()
{
	assert( IsValid() );

	int iResult;

	bool bContinue = true;

	int iRowIndex = 0;

	while( bContinue && ( ( iResult = sqlite3_step( m_pStatement ) ) != SQLITE_DONE ) )
	{
		switch( iResult )
		{
		case SQLITE_BUSY:
			//TODO: handle transactions if needed. - Solokiller
			continue;

		case SQLITE_ROW:
			if( m_pRowCallback )
			{
				m_bHandlingRow = true;
				m_bCallbackInvoked = false;

				CASSQLiteRow row( *this, iRowIndex++ );

				m_pConnection->GetThreadPool().GetThreadQueue().AddItem( &row, m_pRowCallback );

				//Let the callback complete first. - Solokiller
				while( !m_bCallbackInvoked )
				{
					std::this_thread::yield();
				}

				m_bHandlingRow = false;
			}

			continue;

		default:
			m_pConnection->GetThreadPool().GetThreadQueue().AddLogMessage( "SQLitePreparedStatement::Execute: %s\n", sqlite3_errstr( iResult ) );
			bContinue = false;
			break;
		}
	}

	if( m_pRowCallback )
	{
		m_pRowCallback->Release();
		m_pRowCallback = nullptr;
	}

	m_bExecuting = false;
}

bool CASSQLitePreparedStatement::IsValid() const
{
	return m_pStatement != nullptr;
}

void CASSQLitePreparedStatement::BindNull( uint32_t uiIndex )
{
	const int iResult = sqlite3_bind_null( m_pStatement, uiIndex );

	if( SQLITE_OK != iResult )
	{
		m_pConnection->GetLogFunction()( "%s\n", sqlite3_errstr( iResult ) );
	}
}

void CASSQLitePreparedStatement::BindSigned32( uint32_t uiIndex, int32_t iValue )
{
	const int iResult = sqlite3_bind_int( m_pStatement, uiIndex, iValue );

	if( SQLITE_OK != iResult )
	{
		m_pConnection->GetLogFunction()( "%s\n", sqlite3_errstr( iResult ) );
	}
}

void CASSQLitePreparedStatement::BindSigned64( uint32_t uiIndex, int64_t iValue )
{
	const int iResult = sqlite3_bind_int64( m_pStatement, uiIndex, iValue );

	if( SQLITE_OK != iResult )
	{
		m_pConnection->GetLogFunction()( "%s\n", sqlite3_errstr( iResult ) );
	}
}

void CASSQLitePreparedStatement::BindDouble( uint32_t uiIndex, double flValue )
{
	const int iResult = sqlite3_bind_double( m_pStatement, uiIndex, flValue );

	if( SQLITE_OK != iResult )
	{
		m_pConnection->GetLogFunction()( "%s\n", sqlite3_errstr( iResult ) );
	}
}

void CASSQLitePreparedStatement::BindString( uint32_t uiIndex, const std::string& szString )
{
	const size_t uiLength = szString.length();

	char* pszString = new char[ uiLength + 1 ];

	strcpy( pszString, szString.c_str() );

	const int iResult = sqlite3_bind_text( m_pStatement, uiIndex, pszString, -1, ::operator delete[] );

	if( SQLITE_OK != iResult )
	{
		m_pConnection->GetLogFunction()( "%s\n", sqlite3_errstr( iResult ) );
	}
}

bool CASSQLitePreparedStatement::ExecuteStatement( asIScriptFunction* pRowCallback, asIScriptFunction* pCallback )
{
	if( m_bExecuting )
		return false;

	bool bSuccess = false;

	if( m_pConnection->GetThreadPool().AddItem( this, pCallback ) )
	{
		m_bExecuting = true;
		m_pRowCallback = pRowCallback;

		bSuccess = true;
	}
	else
	{
		if( pRowCallback )
			pRowCallback->Release();
	}

	if( pCallback )
		pCallback->Release();

	return bSuccess;
}

void CASSQLitePreparedStatement::CASSQLiteRow::CallbackInvoked()
{
	m_Statement.m_bCallbackInvoked = true;
}

uint32_t CASSQLitePreparedStatement::CASSQLiteRow::GetRowIndex() const
{
	return m_uiRowIndex;
}

uint32_t CASSQLitePreparedStatement::CASSQLiteRow::GetColumnCount() const
{
	return sqlite3_column_count( m_Statement.GetStatement() );
}

int CASSQLitePreparedStatement::CASSQLiteRow::GetColumnType( const uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return SQLITE_NULL;
	}

	return sqlite3_column_type( m_Statement.GetStatement(), uiColumn );
}

bool CASSQLitePreparedStatement::CASSQLiteRow::IsColumnNull( const uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return true;
	}

	return sqlite3_column_type( m_Statement.GetStatement(), uiColumn ) == SQLITE_NULL;
}

int32_t CASSQLitePreparedStatement::CASSQLiteRow::GetSigned32( uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_int( m_Statement.GetStatement(), uiColumn );
}

int64_t CASSQLitePreparedStatement::CASSQLiteRow::GetSigned64( uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_int64( m_Statement.GetStatement(), uiColumn );
}

double CASSQLitePreparedStatement::CASSQLiteRow::GetDouble( uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_double( m_Statement.GetStatement(), uiColumn );
}

std::string CASSQLitePreparedStatement::CASSQLiteRow::GetString( uint32_t uiColumn ) const
{
	if( uiColumn >= GetColumnCount() )
	{
		return "";
	}

	const unsigned char* pszString = sqlite3_column_text( m_Statement.GetStatement(), uiColumn );

	if( pszString )
		return reinterpret_cast<const char*>( pszString );
	else
		return "";
}