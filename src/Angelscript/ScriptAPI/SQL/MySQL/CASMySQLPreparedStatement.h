#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncCommand.h"

#include "CASMySQLBind.h"

class asIScriptFunction;

class CASDateTime;
class CASTime;
class CASMySQLConnection;

class CASMySQLPreparedStatement final : public IASSQLASyncCommand, public CASAtomicRefCountedBaseClass
{
protected:
	friend class CASMySQLResultSet;

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
	*	@return The current parameter count. May not match the query's parameter count.
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
	*	Binds a string.
	*	@param iIndex Parameter index.
	*	@param szString String.
	*/
	void BindString( int iIndex, const std::string& szString );

	/**
	*	Binds a date instance.
	*	@param iIndex Parameter index.
	*	@param date Date instance.
	*/
	void BindDate( int iIndex, const CASDateTime& date );

	/**
	*	Binds a time instance.
	*	@param iIndex Parameter index.
	*	@param time Time instance.
	*/
	void BindTime( int iIndex, const CASTime& time );

	/**
	*	Binds a date time instance.
	*	@param iIndex Parameter index.
	*	@param dateTime Date time instance.
	*/
	void BindDateTime( int iIndex, const CASDateTime& dateTime );

	bool ExecuteStatement( asIScriptFunction* pResultSetCallback = nullptr, asIScriptFunction* pCallback = nullptr );

	CASMySQLConnection* GetConnection() { return m_pConnection; }

private:
	/**
	*	Checks if the bind list size is large enough for the given index. Resizes it if needed.
	*	@return Whether the list could be resized.
	*/
	bool CheckBindListSize( int iIndex );

private:
	CASMySQLConnection* m_pConnection = nullptr;

	std::string m_szStatement;

	std::vector<MYSQL_BIND> m_Binds;
	//Declare these after binds so they destruct first. They might access the binds - Solokiller
	std::vector<CASMySQLBind> m_Variables;

	asIScriptFunction* m_pCallback = nullptr;

	std::atomic<bool> m_bHandledResultSet = false;

private:
	CASMySQLPreparedStatement( const CASMySQLPreparedStatement& ) = delete;
	CASMySQLPreparedStatement& operator=( const CASMySQLPreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H