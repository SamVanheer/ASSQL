#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H

#include <queue>
#include <mutex>

#include "ASSQLThreading.h"

class asIScriptContext;

/**
*	A thread queue for Angelscript SQL items.
*/
class CASSQLThreadQueue final
{
private:
	using Queue_t = std::queue<CASSQLItem>;

public:
	CASSQLThreadQueue() = default;
	~CASSQLThreadQueue();

	/**
	*	Adds an item to the queue.
	*	@param pItem Item to add. The item's reference counting shall be thread-safe.
	*	@param pCallback Callback to invoke after completion.
	*/
	bool AddItem( IASSQLASyncItem* pItem, asIScriptFunction* pCallback );

	/**
	*	Clears all items from the queue.
	*/
	void Clear();

	/**
	*	Processes the queue of items.
	*	@param context Context to use for callbacks.
	*/
	void ProcessQueue( asIScriptContext& context );

private:
	std::mutex m_Mutex;

	Queue_t m_Queue;

private:
	CASSQLThreadQueue( const CASSQLThreadQueue& ) = delete;
	CASSQLThreadQueue& operator=( const CASSQLThreadQueue& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H