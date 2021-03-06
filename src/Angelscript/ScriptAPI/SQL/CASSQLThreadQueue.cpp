#include <cassert>
#include <cstdarg>
#include <cstring>

#include <angelscript.h>

#include <Angelscript/wrapper/ASCallable.h>

#include "IASSQLASyncItem.h"

#include "CASSQLThreadQueue.h"

CASSQLThreadQueue::CASSQLThreadQueue( ASSQLLogFunction pLogFunction )
	: m_pLogFunction( pLogFunction )
{
	assert( pLogFunction );
}

CASSQLThreadQueue::~CASSQLThreadQueue()
{
	Clear();
}

size_t CASSQLThreadQueue::GetQueueSize() const
{
	std::lock_guard<std::mutex> guard( m_Mutex );

	return m_Queue.size();
}

bool CASSQLThreadQueue::AddItem( SQLQueryResult::SQLQueryResult result, IASSQLASyncItem* pItem, asIScriptFunction* pCallback )
{
	assert( pItem );
	assert( pCallback );

	if( !pItem || !pCallback )
		return false;

	std::lock_guard<std::mutex> guard( m_Mutex );

	m_Queue.push( { result, pItem, pCallback } );

	pItem->AddRef();

	pCallback->AddRef();

	return true;
}

void CASSQLThreadQueue::Clear()
{
	std::lock_guard<std::mutex> guard( m_Mutex );

	while( !m_Queue.empty() )
	{
		auto& entry = m_Queue.front();

		entry.pCallback->Release();

		entry.pItem->Release();

		m_Queue.pop();
	}
}

bool CASSQLThreadQueue::ProcessQueue( asIScriptContext& context )
{
	{
		std::lock_guard<std::mutex> guard( m_LogMutex );

		//This avoids allocating a string if it's empty.
		if( m_LogMessages.rdbuf()->in_avail() )
		{
			const std::string szString = m_LogMessages.str();
			m_pLogFunction( "%s", szString.c_str() );
			m_LogMessages.str( "" );
		}
	}

	std::lock_guard<std::mutex> guard( m_Mutex );

	const bool bWorkDone = !m_Queue.empty();

	while( !m_Queue.empty() )
	{
		auto& entry = m_Queue.front();

		as::Call( &context, entry.pCallback, entry.result, entry.pItem );

		entry.pItem->CallbackInvoked();

		entry.pCallback->Release();

		entry.pItem->Release();

		m_Queue.pop();
	}

	return bWorkDone;
}

void CASSQLThreadQueue::AddLogMessage( const char* const pszFormat, ... )
{
	assert( pszFormat );

	char szBuffer[ 4096 ];

	va_list list;

	va_start( list, pszFormat );
	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );
	va_end( list );

	if( iResult >= 0 && static_cast<size_t>( iResult ) < sizeof( szBuffer ) )
	{
		std::lock_guard<std::mutex> guard( m_LogMutex );
		m_LogMessages << szBuffer;
	}
}
