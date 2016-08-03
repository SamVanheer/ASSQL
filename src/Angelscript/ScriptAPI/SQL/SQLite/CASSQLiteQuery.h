#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H

#include <string>

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLQuery.h"

class CASSQLiteConnection;

class CASSQLiteQuery final : public IASSQLQuery, public CASAtomicRefCountedBaseClass
{
public:
	CASSQLiteQuery( CASSQLiteConnection* pConnection, const char* const pszQuery );
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

	void Execute() override;

	bool IsValid() const override { return true; }

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	std::string m_szQuery;

private:
	CASSQLiteQuery( const CASSQLiteQuery& ) = delete;
	CASSQLiteQuery& operator=( const CASSQLiteQuery& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEQUERY_H