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
		CASSQLiteRow( CASSQLitePreparedStatement& statement, const uint32_t uiRowIndex )
			: m_Statement( statement )
			, m_uiRowIndex( uiRowIndex )
		{
		}

		void AddRef() const override {}

		void Release() const override {}

		void Execute() override {}

		void CallbackInvoked() override;

		/**
		*	@return The row index.
		*/
		uint32_t GetRowIndex() const;

		/**
		*	@return The column count.
		*/
		uint32_t GetColumnCount() const;

		/**
		*	@return The type of the given column. Invalid column indices are always null.
		*/
		int GetColumnType( const uint32_t uiColumn ) const;

		/**
		*	@return Whether the given column is null. Invalid column indices are always null.
		*/
		bool IsColumnNull( const uint32_t uiColumn ) const;

		/**
		*	@return A column integer.
		*/
		int32_t GetSigned32( uint32_t uiColumn ) const;

		/**
		*	@return A column 64 bit integer.
		*/
		int64_t GetSigned64( uint32_t uiColumn ) const;

		/**
		*	@return A column double.
		*/
		double GetDouble( uint32_t uiColumn ) const;

		/**
		*	@return A column string.
		*/
		std::string GetString( uint32_t uiColumn ) const;

	private:
		CASSQLitePreparedStatement& m_Statement;
		const uint32_t m_uiRowIndex;

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

	/**
	*	Binds a null value.
	*	@param uiIndex parameter to bind.
	*/
	void BindNull( uint32_t uiIndex );

	/**
	*	Binds a signed 32 bit int value.
	*	@param uiIndex parameter to bind.
	*/
	void BindSigned32( uint32_t uiIndex, int32_t iValue );

	/**
	*	Binds a signed 64 bit int value.
	*	@param uiIndex parameter to bind.
	*/
	void BindSigned64( uint32_t uiIndex, int64_t iValue );

	/**
	*	Binds a 64 bit float value.
	*	@param uiIndex parameter to bind.
	*/
	void BindDouble( uint32_t uiIndex, double flValue );

	/**
	*	Binds a string value.
	*	@param uiIndex parameter to bind.
	*/
	void BindString( uint32_t uiIndex, const std::string& szString );

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

private:
	CASSQLiteConnection* m_pConnection = nullptr;
	sqlite3_stmt* m_pStatement = nullptr;

	std::atomic<bool> m_bExecuting = false;
	std::atomic<bool> m_bCallbackInvoked = false;

	asIScriptFunction* m_pRowCallback = nullptr;

private:
	CASSQLitePreparedStatement( const CASSQLitePreparedStatement& ) = delete;
	CASSQLitePreparedStatement& operator=( const CASSQLitePreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITEPREPAREDSTATEMENT_H