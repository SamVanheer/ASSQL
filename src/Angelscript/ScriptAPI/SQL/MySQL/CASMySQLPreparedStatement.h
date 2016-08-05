#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H

#include <cstdint>
#include <string>
#include <vector>

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

		void Set( enum_field_types type, MYSQL_BIND* pBind, const char* const pBuffer = nullptr, const size_t uiLength = 0 );

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

			uint8_t		m_uiVal8;
			uint16_t	m_uiVal16;
			uint32_t	m_uiVal32;
			uint64_t	m_uiVal64;

			float		m_flValue;
			double		m_dValue;
		};

		std::vector<char> m_Buffer;

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
	*	Binds a null parameter.
	*	@param iIndex Parameter index.
	*/
	void BindNull( int iIndex );

	/**
	*	Binds a boolean.
	*	@param iIndex Parameter index.
	*	@param bValue Value.
	*/
	void BindBoolean( int iIndex, bool bValue );

	/**
	*	Binds an 8 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned8( int iIndex, int8_t iValue );

	/**
	*	Binds an 8 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned8( int iIndex, uint8_t uiValue );

	/**
	*	Binds a 16 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned16( int iIndex, int16_t iValue );

	/**
	*	Binds a 16 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned16( int iIndex, uint16_t uiValue );

	/**
	*	Binds a 32 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned32( int iIndex, int32_t iValue );

	/**
	*	Binds a 32 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned32( int iIndex, uint32_t uiValue );

	/**
	*	Binds a 64 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned64( int iIndex, int64_t iValue );

	/**
	*	Binds a 64 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned64( int iIndex, uint64_t uiValue );

	/**
	*	Binds a 32 bit float.
	*	@param iIndex Parameter index.
	*	@param flValue Value.
	*/
	void BindFloat( int iIndex, float flValue );

	/**
	*	Binds a 64 bit float.
	*	@param iIndex Parameter index.
	*	@param flValue Value.
	*/
	void BindDouble( int iIndex, double flValue );

	/**
	*	Binds a text string.
	*	@param iIndex Parameter index.
	*	@param szText Text.
	*/
	void BindText( int iIndex, const std::string& szText );

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