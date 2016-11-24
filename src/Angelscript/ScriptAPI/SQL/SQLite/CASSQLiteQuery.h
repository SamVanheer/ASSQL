#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H

#include <string>

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncCommand.h"

class CASSQLiteConnection;

/**
*	SQLite query. Executes queries without processing any results.
*/
class CASSQLiteQuery final : public IASSQLASyncCommand, public CASAtomicRefCountedBaseClass
{
public:
	/**
	*	Constructor.
	*	@param pConnection The connection that created this query.
	*	@param pszQuery Query to execute.
	*/
	CASSQLiteQuery( CASSQLiteConnection* pConnection, const char* const pszQuery );

	/**
	*	Destructor.
	*/
	~CASSQLiteQuery();

	void AddRef() const override final
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const override final
	{
		if( InternalRelease() )
			delete this;
	}

	SQLQueryResult::SQLQueryResult Execute() override;

	bool IsValid() const { return true; }

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	std::string m_szQuery;

private:
	CASSQLiteQuery( const CASSQLiteQuery& ) = delete;
	CASSQLiteQuery& operator=( const CASSQLiteQuery& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H