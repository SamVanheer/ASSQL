#include <cassert>

#include <iostream>

#include <angelscript.h>

#include <Angelscript/wrapper/ASCallable.h>

#include "IASSQLASyncItem.h"

#include "CASSQLThreadQueue.h"

CASSQLThreadQueue::~CASSQLThreadQueue()
{
	Clear();
}

bool CASSQLThreadQueue::AddItem( IASSQLASyncItem* pItem, asIScriptFunction* pCallback )
{
	assert( pItem );
	assert( pCallback );

	if( !pItem || !pCallback )
		return false;

	std::lock_guard<std::mutex> guard( m_Mutex );

	m_Queue.push( { pItem, pCallback } );

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

void CASSQLThreadQueue::ProcessQueue( asIScriptContext& context )
{
	{
		std::lock_guard<std::mutex> guard( m_LogMutex );

		for( const auto& szMessage : m_LogMessages )
		{
			std::cout << szMessage << std::endl;
		}

		m_LogMessages.clear();
	}

	std::lock_guard<std::mutex> guard( m_Mutex );

	while( !m_Queue.empty() )
	{
		auto& entry = m_Queue.front();

		as::Call( &context, entry.pCallback, entry.pItem );

		entry.pItem->CallbackInvoked();

		entry.pCallback->Release();

		entry.pItem->Release();

		m_Queue.pop();
	}
}

void CASSQLThreadQueue::AddLogMessage( const char* const pszMessage )
{
	assert( pszMessage );

	AddLogMessage( std::string( pszMessage ) );
}

void CASSQLThreadQueue::AddLogMessage( std::string&& szMessage )
{
	std::lock_guard<std::mutex> guard( m_LogMutex );

	m_LogMessages.emplace_back( std::move( szMessage ) );
}