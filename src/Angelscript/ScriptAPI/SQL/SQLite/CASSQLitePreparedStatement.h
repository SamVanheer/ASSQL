#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H

#include <atomic>
#include <string>

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncItem.h"
#include "../IASSQLRow.h"

class CASSQLiteConnection;

class CASSQLitePreparedStatement final : public IASSQLASyncItem, public CASAtomicRefCountedBaseClass
{
private:

	class CASSQLiteRow final : public IASSQLRow
	{
	public:
		CASSQLiteRow( CASSQLitePreparedStatement& statement, const int iRowIndex )
			: m_Statement( statement )
			, m_iRowIndex( iRowIndex )
		{
		}

		void AddRef() const override {}

		void Release() const override {}

		void Execute() override {}

		void CallbackInvoked() override;

		int GetRowIndex() const;

		int GetColumnCount() const override;

		ASSQLDataType GetColumnType( const int iColumn ) const override;

		bool IsColumnNull( const int iColumn ) const override;

		int GetColumnInt( int iColumn ) const override;

		double GetColumnDouble( int iColumn ) const override;

		std::string GetColumnString( int iColumn ) const override;

	private:
		CASSQLitePreparedStatement& m_Statement;
		const int m_iRowIndex;

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

	bool IsValid() const;

	void Bind( int iIndex, int iValue );

	void Bind( int iIndex, double flValue );

	bool ExecuteStatement( asIScriptFunction* pRowCallback = nullptr, asIScriptFunction* pCallback = nullptr );

	sqlite3_stmt* GetStatement() { return m_pStatement; }

	bool IsHandlingRow() const { return m_bHandlingRow; }

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	sqlite3_stmt* m_pStatement = nullptr;

	std::atomic<bool> m_bExecuting = false;
	std::atomic<bool> m_bCallbackInvoked = false;
	std::atomic<bool> m_bHandlingRow = false;

	asIScriptFunction* m_pRowCallback = nullptr;
	asIScriptFunction* m_pCallback = nullptr;

private:
	CASSQLitePreparedStatement( const CASSQLitePreparedStatement& ) = delete;
	CASSQLitePreparedStatement& operator=( const CASSQLitePreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H