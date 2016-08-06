#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H

#include <ctpl_stl.h>

#include "CASSQLThreadQueue.h"

class IASSQLASyncCommand;
class asIScriptFunction;
class asIScriptContext;

/**
*	A thread pool for Angelscript SQL commands.
*/
class CASSQLThreadPool final
{
private:
	struct CASSQLCommand final
	{
		IASSQLASyncCommand* pCommand;
		asIScriptFunction* pCallback;
	};

public:
	CASSQLThreadPool( const size_t uiNumThreads, ASSQLLogFunction pLogFunction );
	~CASSQLThreadPool() = default;

	CASSQLThreadQueue& GetThreadQueue() { return m_Queue; }

	/**
	*	@return The log function.
	*/
	ASSQLLogFunction GetLogFunction() const { return m_Queue.GetLogFunction(); }

	bool ThreadsActive();

	/**
	*	Adds an item to the queue.
	*	@param pCommand Command to add. The command's reference counting shall be thread-safe.
	*	@param pCallback Callback to invoke after completion.
	*/
	bool AddItem( IASSQLASyncCommand* pCommand, asIScriptFunction* pCallback );

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
	static void ExecuteItem( int iID, CASSQLThreadPool* pPool, CASSQLCommand command );

private:
	ctpl::thread_pool m_Pool;

	CASSQLThreadQueue m_Queue;

private:
	CASSQLThreadPool( const CASSQLThreadPool& ) = delete;
	CASSQLThreadPool& operator=( const CASSQLThreadPool& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADPOOL_H