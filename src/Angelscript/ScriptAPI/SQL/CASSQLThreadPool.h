#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H

#include <ctpl_stl.h>

#include "ASSQLThreading.h"

#include "CASSQLThreadQueue.h"

class IASSQLASyncItem;
class asIScriptFunction;
class asIScriptContext;

/**
*	A thread pool for Angelscript SQL items.
*/
class CASSQLThreadPool final
{
private:
public:
	CASSQLThreadPool( const size_t uiNumThreads );
	~CASSQLThreadPool() = default;

	CASSQLThreadQueue& GetThreadQueue() { return m_Queue; }

	bool ThreadsActive();

	/**
	*	Adds an item to the queue.
	*	@param pItem Item to add. The item's reference counting shall be thread-safe.
	*	@param pCallback Callback to invoke after completion.
	*/
	bool AddItem( IASSQLASyncItem* pItem, asIScriptFunction* pCallback );

	/**
	*	Process the queue's items.
	*/
	bool ProcessQueue( asIScriptContext& context );

	/**
	*	Stops all threads.
	*	@param bWait If true, will wait for all threads to finish before stopping. Otherwise, terminates all threads.
	*/
	void Stop( const bool bWait = false );

private:
	static void ExecuteItem( int iID, CASSQLThreadPool* pPool, CASSQLItem item );

private:
	ctpl::thread_pool m_Pool;

	CASSQLThreadQueue m_Queue;

private:
	CASSQLThreadPool( const CASSQLThreadPool& ) = delete;
	CASSQLThreadPool& operator=( const CASSQLThreadPool& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H