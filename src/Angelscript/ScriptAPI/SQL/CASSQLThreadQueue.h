#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H

#include <queue>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "ASSQL.h"

class asIScriptContext;

class IASSQLASyncItem;
class asIScriptFunction;

/**
*	A thread queue for Angelscript SQL items.
*/
class CASSQLThreadQueue final
{
private:
	struct CASSQLItem final
	{
		IASSQLASyncItem* pItem;
		asIScriptFunction* pCallback;
	};

	using Queue_t = std::queue<CASSQLItem>;

public:
	CASSQLThreadQueue( ASSQLLogFunction pLogFunction );
	~CASSQLThreadQueue();

	/**
	*	@return The log function.
	*/
	ASSQLLogFunction GetLogFunction() const { return m_pLogFunction; }

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
	*	@param pszFormat Format string.
	*	@param ... Arguments.
	*/
	void AddLogMessage( const char* const pszFormat, ... );

private:
	ASSQLLogFunction m_pLogFunction;

	std::mutex m_Mutex;

	Queue_t m_Queue;

	std::mutex m_LogMutex;

	std::stringstream m_LogMessages;

private:
	CASSQLThreadQueue( const CASSQLThreadQueue& ) = delete;
	CASSQLThreadQueue& operator=( const CASSQLThreadQueue& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_CASSQLTHREADQUEUE_H