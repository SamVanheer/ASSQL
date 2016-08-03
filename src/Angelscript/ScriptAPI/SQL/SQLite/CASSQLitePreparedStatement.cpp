#include <cassert>

#include "../CASSQLThreadPool.h"

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

	while( bContinue && ( ( iResult = sqlite3_step( m_pStatement ) ) != SQLITE_DONE ) )
	{
		switch( iResult )
		{
		case SQLITE_BUSY:
			//TODO: handle transactions if needed. - Solokiller
			continue;

		case SQLITE_ROW:
			//TODO: handle rows. - Solokiller
			continue;

		default:
			//TODO: handle specific errors as needed. - Solokiller
			bContinue = false;
			break;
		}
	}
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

void CASSQLitePreparedStatement::ExecuteStatement( asIScriptFunction* pCallback )
{
	//TODO: handle multiple calls. - Solokiller

	m_pConnection->GetThreadPool().AddItem( this, pCallback );

	if( pCallback )
		pCallback->Release();
}