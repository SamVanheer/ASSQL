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

void CASSQLitePreparedStatement::Bind( int iIndex, int iValue )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_int( m_pStatement, iIndex, iValue );
}

void CASSQLitePreparedStatement::Bind( int iIndex, double flValue )
{
	//TODO: error handling - Solokiller
	sqlite3_bind_double( m_pStatement, iIndex, flValue );
}

void CASSQLitePreparedStatement::ExecuteStatement( asIScriptFunction* pRowCallback, asIScriptFunction* pCallback )
{
	if( m_bExecuting )
		return;

	if( m_pConnection->GetThreadPool().AddItem( this, pCallback ) )
	{
		m_bExecuting = true;
		m_pRowCallback = pRowCallback;
		m_pCallback = pCallback;
	}
	else
	{
		if( pRowCallback )
			pRowCallback->Release();

		if( pCallback )
			pCallback->Release();
	}
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

int CASSQLitePreparedStatement::CASSQLiteRow::GetColumnInt( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_int( m_Statement.GetStatement(), iColumn );
}

double CASSQLitePreparedStatement::CASSQLiteRow::GetColumnDouble( int iColumn ) const
{
	if( iColumn < 0 || iColumn >= GetColumnCount() )
	{
		return 0;
	}

	return sqlite3_column_double( m_Statement.GetStatement(), iColumn );
}