#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H

#include <cstdint>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncItem.h"

class asIScriptFunction;

class CASMySQLConnection;

class CASMySQLPreparedStatement final : public IASSQLASyncItem, public CASAtomicRefCountedBaseClass
{
private:
	class Variable final
	{
	public:
		Variable() = default;
		~Variable();

		void Set( enum_field_types type, MYSQL_BIND* pBind );

		void Clear();

		MYSQL_BIND* m_pBind = nullptr;

		my_bool m_bIsNull = false;
		my_bool m_bError = false;

		union
		{
			int8_t		m_iVal8;
			int16_t		m_iVal16;
			int32_t		m_iVal32;
			int64_t		m_iVal64;

			float		m_flValue;
			double		m_dValue;
		};

	private:
		Variable( const Variable& ) = delete;
		Variable& operator=( const Variable& ) = delete;
	};

public:
	CASMySQLPreparedStatement( CASMySQLConnection* pConnection, const char* const pszStatement );
	~CASMySQLPreparedStatement();

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
	*	@return Whether the statement is valid.
	*/
	bool IsValid() const;

	/**
	*	@return The parameter count.
	*/
	int GetParamCount() const;

	/**
	*	Binds a 32 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void Bind( int iIndex, int iValue );

	bool ExecuteStatement( asIScriptFunction* pCallback = nullptr );

private:
	CASMySQLConnection* m_pConnection = nullptr;
	MYSQL_STMT* m_pStatement = nullptr;

	Variable* m_pVariables = nullptr;
	MYSQL_BIND* m_pBinds = nullptr;

private:
	CASMySQLPreparedStatement( const CASMySQLPreparedStatement& ) = delete;
	CASMySQLPreparedStatement& operator=( const CASMySQLPreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H