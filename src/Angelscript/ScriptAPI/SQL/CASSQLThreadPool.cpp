#include <cassert>

#include <angelscript.h>

#include "IASSQLASyncCommand.h"

#include "CASSQLThreadPool.h"

CASSQLThreadPool::CASSQLThreadPool( const size_t uiNumThreads, ASSQLLogFunction pLogFunction )
	: m_Pool( uiNumThreads )
	, m_Queue( pLogFunction )
{
}

bool CASSQLThreadPool::ThreadsActive()
{
	return m_Pool.has_work() || ( m_Pool.size() - m_Pool.n_idle() > 0 );
}

bool CASSQLThreadPool::AddItem( IASSQLASyncCommand* pCommand, asIScriptFunction* pCallback )
{
	assert( pCommand );

	if( !pCommand )
		return false;

	pCommand->AddRef();

	if( pCallback )
		pCallback->AddRef();

	m_Pool.push( &CASSQLThreadPool::ExecuteItem, this, CASSQLCommand{ pCommand, pCallback } );

	return true;
}

bool CASSQLThreadPool::ProcessQueue( asIScriptContext& context )
{
	return m_Queue.ProcessQueue( context );
}

void CASSQLThreadPool::Stop( const bool bWait )
{
	m_Pool.stop( bWait );
}

void CASSQLThreadPool::ExecuteItem( int iID, CASSQLThreadPool* pPool, CASSQLCommand command )
{
	command.pCommand->Execute();

	if( command.pCallback )
	{
		//Post the command for processing by the callback.
		pPool->m_Queue.AddItem( command.pCommand, command.pCallback );

		command.pCallback->Release();
	}

	command.pCommand->Release();
}