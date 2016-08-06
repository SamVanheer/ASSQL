#include <cassert>

#include <angelscript.h>

#include "IASSQLASyncItem.h"

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

bool CASSQLThreadPool::AddItem( IASSQLASyncItem* pItem, asIScriptFunction* pCallback )
{
	assert( pItem );

	if( !pItem )
		return false;

	pItem->AddRef();

	if( pCallback )
		pCallback->AddRef();

	m_Pool.push( &CASSQLThreadPool::ExecuteItem, this, CASSQLItem{ pItem, pCallback } );

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

void CASSQLThreadPool::ExecuteItem( int iID, CASSQLThreadPool* pPool, CASSQLItem item )
{
	item.pItem->Execute();

	if( item.pCallback )
	{
		//Post the item for processing by the callback.
		pPool->m_Queue.AddItem( item.pItem, item.pCallback );

		item.pCallback->Release();
	}

	item.pItem->Release();
}