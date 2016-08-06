#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H

#include <queue>
#include <mutex>
#include <string>
#include <vector>

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
	*	@return Whether any work was done.
	*/
	bool ProcessQueue( asIScriptContext& context );

	/**
	*	Adds a log message to be output in the main thread.
	*/
	void AddLogMessage( const char* const pszMessage );

	/**
	*	@copydoc AddLogMessage( const char* const pszMessage )
	*/
	//TODO: replace with a fixed size buffer, use varargs - Solokiller
	void AddLogMessage( std::string&& szMessage );

private:
	std::mutex m_Mutex;

	Queue_t m_Queue;

	std::mutex m_LogMutex;

	std::vector<std::string> m_LogMessages;

private:
	CASSQLThreadQueue( const CASSQLThreadQueue& ) = delete;
	CASSQLThreadQueue& operator=( const CASSQLThreadQueue& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H