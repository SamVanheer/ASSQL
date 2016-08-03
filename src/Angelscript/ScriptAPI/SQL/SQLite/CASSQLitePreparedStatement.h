#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H

#include <atomic>

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLPreparedStatement.h"
#include "../IASSQLRow.h"

class CASSQLiteConnection;

class CASSQLitePreparedStatement final : public IASSQLPreparedStatement, public CASAtomicRefCountedBaseClass
{
private:

	class CASSQLiteRow final : public IASSQLRow
	{
	public:
		CASSQLiteRow( CASSQLitePreparedStatement& statement )
			: m_Statement( statement )
		{
		}

		void AddRef() const override {}

		void Release() const override {}

		void Execute() override {}

		void CallbackInvoked() override;

		int GetColumnCount() const override;

		int GetColumnInt( int iColumn ) const override;

	private:
		CASSQLitePreparedStatement& m_Statement;

	private:
		CASSQLiteRow( const CASSQLiteRow& ) = delete;
		CASSQLiteRow& operator=( const CASSQLiteRow& ) = delete;
	};

	friend class CASSQLiteRow;

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

	void ExecuteStatement( asIScriptFunction* pRowCallback = nullptr, asIScriptFunction* pCallback = nullptr ) override;

	sqlite3_stmt* GetStatement() { return m_pStatement; }

	bool IsHandlingRow() const { return m_bHandlingRow; }

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	sqlite3_stmt* m_pStatement = nullptr;

	std::atomic<bool> m_bCallbackInvoked = false;
	std::atomic<bool> m_bHandlingRow = false;

	asIScriptFunction* m_pRowCallback = nullptr;
	asIScriptFunction* m_pCallback = nullptr;

private:
	CASSQLitePreparedStatement( const CASSQLitePreparedStatement& ) = delete;
	CASSQLitePreparedStatement& operator=( const CASSQLitePreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H