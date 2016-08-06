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
	//TODO: log warning if these are still non-null here. - Solokiller
	if( m_pRowCallback )
	{
		m_pRowCallback->Release();
		m_pRowCallback = nullptr;
	}

	if( m_pCallback )
	{
		m_pCallback->Release();
		m_pCallback = nullptr;
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
			//TODO: handle specific errors as needed. - Solokiller
			bContinue = false;
			break;
		}
	}

	if( m_pRowCallback )
	{
		m_pRowCallback->Release();
		m_pRowCallback = nullptr;
	}

	if( m_pCallback )
	{
		m_pCallback->Release();
		m_pCallback = nullptr;
	}

	m_bExecuting = false;
}

bool CASSQLitePreparedStatement::IsValid() const
{
	return m_pStatement != nullptr;
}

void CASSQLitePreparedStatement::BindNull( int iIndex )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_null( m_pStatement, iIndex );
}

void CASSQLitePreparedStatement::BindSigned32( int iIndex, int32_t iValue )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_int( m_pStatement, iIndex, iValue );
}

void CASSQLitePreparedStatement::BindSigned64( int iIndex, int64_t iValue )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_int64( m_pStatement, iIndex, iValue );
}

void CASSQLitePreparedStatement::BindDouble( int iIndex, double flValue )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_double( m_pStatement, iIndex, flValue );
}

void CASSQLitePreparedStatement::BindString( int iIndex, const std::string& szString )
{
	const size_t uiLength = szString.length();

	char* pszString = new char[ uiLength + 1 ];

	strcpy( pszString, szString.c_str() );

	sqlite3_bind_text( m_pStatement, iIndex, pszString, -1, ::operator delete[] );
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
		//TODO: don't have to store this callback - Solokiller
		m_pCallback = pCallback;

		bSuccess = true;
	}
	else
	{
		if( pRowCallback )
			pRowCallback->Release();

		if( pCallback )
			pCallback->Release();
	}

	return bSuccess;
}

void CASSQLitePreparedStatement::CASSQLiteRow::CallbackInvoked()
{
	m_Statement.m_bCallbackInvoked = true;
}

int CASSQLitePreparedStatement::CASSQLiteRow::GetRowIndex() const
{
	return m_iRowIndex;
}

int CASSQLitePreparedStatement::CASSQLiteRow::GetColumnCount() const
{
	return sqlite3_column_count( m_Statement.GetStatement() );
}

int CASSQLitePreparedStatement::CASSQLiteRow::GetColumnType( const int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return SQLITE_NULL;
	}

	return sqlite3_column_type( m_Statement.GetStatement(), iColumn );
}

bool CASSQLitePreparedStatement::CASSQLiteRow::IsColumnNull( const int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return true;
	}

	return sqlite3_column_type( m_Statement.GetStatement(), iColumn ) == SQLITE_NULL;
}

int32_t CASSQLitePreparedStatement::CASSQLiteRow::GetSigned32( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_int( m_Statement.GetStatement(), iColumn );
}

int64_t CASSQLitePreparedStatement::CASSQLiteRow::GetSigned64( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_int64( m_Statement.GetStatement(), iColumn );
}

double CASSQLitePreparedStatement::CASSQLiteRow::GetDouble( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_double( m_Statement.GetStatement(), iColumn );
}

std::string CASSQLitePreparedStatement::CASSQLiteRow::GetString( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return "";
	}

	const unsigned char* pszString = sqlite3_column_text( m_Statement.GetStatement(), iColumn );

	if( pszString )
		return reinterpret_cast<const char*>( pszString );
	else
		return "";
}