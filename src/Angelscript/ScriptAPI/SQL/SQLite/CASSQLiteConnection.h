#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../CASSQLThreadPool.h"

#include "../IASSQLConnection.h"

#include "CASSQLitePreparedStatement.h"

/**
*	Angelscript SQLite connection.
*/
class CASSQLiteConnection : public IASSQLConnection, public CASAtomicRefCountedBaseClass
{
public:
	CASSQLiteConnection( CASSQLThreadPool& pool, const char* const pszFilename );

	/**
	*	Destructor.
	*	Closes the connection if it is still open.
	*/
	~CASSQLiteConnection();

	void AddRef() const override final
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const override final
	{
		if( InternalRelease() )
			delete this;
	}

	bool IsOpen() const override;

	void Close() override;

	bool Query( const std::string& szQuery, asIScriptFunction* const pCallback ) override;

	/**
	*	Creates a prepared statement.
	*	@param szStatement Statement.
	*	@return if the statement is valid, returns the statement. Otherwise, returns null.
	*/
	CASSQLitePreparedStatement* CreatePreparedStatement( const std::string& szStatement );

	/**
	*	@return The thread pool used by this connection.
	*/
	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	/**
	*	@return The log function used by this connection.
	*/
	ASSQLLogFunction GetLogFunction() { return m_ThreadPool.GetLogFunction(); }

	/**
	*	@return The SQLite connection.
	*/
	sqlite3* GetConnection() { return m_pConnection; }

private:
	CASSQLThreadPool& m_ThreadPool;

	sqlite3* m_pConnection = nullptr;

private:
	CASSQLiteConnection( const CASSQLiteConnection& ) = delete;
	CASSQLiteConnection& operator=( const CASSQLiteConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H