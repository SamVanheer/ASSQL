#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLPreparedStatement.h"

class CASSQLiteConnection;

class CASSQLitePreparedStatement final : public IASSQLPreparedStatement, public CASAtomicRefCountedBaseClass
{
public:
	CASSQLitePreparedStatement( CASSQLiteConnection* pConnection, const char* const pszStatement );
	~CASSQLitePreparedStatement();

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

	bool IsValid() const override;

	void Bind( int iIndex, int iValue ) override;

	void ExecuteStatement( asIScriptFunction* pCallback = nullptr ) override;

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	sqlite3_stmt* m_pStatement = nullptr;

private:
	CASSQLitePreparedStatement( const CASSQLitePreparedStatement& ) = delete;
	CASSQLitePreparedStatement& operator=( const CASSQLitePreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H