#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H

#include <atomic>
#include <cstdint>
#include <string>

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncCommand.h"

class CASSQLiteConnection;

/**
*	SQLite prepared statement.
*/
class CASSQLitePreparedStatement final : public IASSQLASyncCommand, public CASAtomicRefCountedBaseClass
{
public:

	/**
	*	SQLite prepared statement row.
	*/
	class CASSQLiteRow final : public IASSQLASyncCommand
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

		/**
		*	@return The row index.
		*/
		int GetRowIndex() const;

		/**
		*	@return The column count.
		*/
		int GetColumnCount() const;

		/**
		*	@return The type of the given column. Invalid column indices are always null.
		*/
		int GetColumnType( const int iColumn ) const;

		/**
		*	@return Whether the given column is null. Invalid column indices are always null.
		*/
		bool IsColumnNull( const int iColumn ) const;

		/**
		*	@return A column integer.
		*/
		int32_t GetSigned32( int iColumn ) const;

		/**
		*	@return A column 64 bit integer.
		*/
		int64_t GetSigned64( int iColumn ) const;

		/**
		*	@return A column double.
		*/
		double GetDouble( int iColumn ) const;

		/**
		*	@return A column string.
		*/
		std::string GetString( int iColumn ) const;

	private:
		CASSQLitePreparedStatement& m_Statement;
		const int m_iRowIndex;

	private:
		CASSQLiteRow( const CASSQLiteRow& ) = delete;
		CASSQLiteRow& operator=( const CASSQLiteRow& ) = delete;
	};

	friend class CASSQLiteRow;

public:
	/**
	*	Constructor.
	*	@param pConnection Connection that created this statement.
	*	@param pszStatement Statement string.
	*/
	CASSQLitePreparedStatement( CASSQLiteConnection* pConnection, const char* const pszStatement );

	/**
	*	Destructor.
	*/
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

	/**
	*	@return Whether this statement is valid.
	*/
	bool IsValid() const;

	void BindNull( int iIndex );

	void BindSigned32( int iIndex, int32_t iValue );

	void BindSigned64( int iIndex, int64_t iValue );

	void BindDouble( int iIndex, double flValue );

	void BindString( int iIndex, const std::string& szString );

	/**
	*	Executes this statement.
	*	@param pRowCallback Optional. Callback to invoke when a row is being handled.
	*	@param pCallback Optional. Callback to invoke when the statement has finished execution and the results have been processed.
	*	@return Whether the statement was successfully queued for execution.
	*/
	bool ExecuteStatement( asIScriptFunction* pRowCallback = nullptr, asIScriptFunction* pCallback = nullptr );

	/**
	*	@return The SQLite statement.
	*/
	sqlite3_stmt* GetStatement() { return m_pStatement; }

	/**
	*	@return Whether this statement is handling a row.
	*/
	bool IsHandlingRow() const { return m_bHandlingRow; }

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	sqlite3_stmt* m_pStatement = nullptr;

	std::atomic<bool> m_bExecuting = false;
	std::atomic<bool> m_bCallbackInvoked = false;
	std::atomic<bool> m_bHandlingRow = false;

	asIScriptFunction* m_pRowCallback = nullptr;

private:
	CASSQLitePreparedStatement( const CASSQLitePreparedStatement& ) = delete;
	CASSQLitePreparedStatement& operator=( const CASSQLitePreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H