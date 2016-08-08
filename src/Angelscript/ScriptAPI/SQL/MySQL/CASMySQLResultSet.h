#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H

#include <cstdint>
#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "Angelscript/ScriptAPI/CASDateTime.h"
#include "Angelscript/ScriptAPI/CASTime.h"

#include "../IASSQLASyncItem.h"

class CASMySQLBind;
class CASMySQLPreparedStatement;

/**
*	MySQL result set.
*/
class CASMySQLResultSet final : public IASSQLASyncItem, public CASAtomicRefCountedBaseClass
{
public:
	/**
	*	Constructor.
	*	@param pStatement The statement that produced this result.
	*	@param pMyStatement. The MySQL statement that produced this result.
	*/
	CASMySQLResultSet( CASMySQLPreparedStatement* pStatement, MYSQL_STMT* pMyStatement );

	/**
	*	Destructor.
	*/
	~CASMySQLResultSet();

private:
	/**
	*	Cleans up the result set.
	*/
	void Destruct();

public:

	void AddRef() const
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	void CallbackInvoked() override;

	/**
	*	@return Whether this result set is valid.
	*/
	bool IsValid() const;

	/**
	*	@return The number of fields in a single row.
	*/
	uint32_t GetFieldCount() const;

	/**
	*	Fetches the next row.
	*	@return Whether there is a row to process, false if no data is left.
	*/
	bool Next();

	/**
	*	@param uiColumn Column to check.
	*	@return Whether the given column is null.
	*/
	bool IsNull( uint32_t uiColumn ) const;

	/**
	*	@param uiColumn Column to retrieve.
	*	@return A boolean.
	*/
	bool GetBoolean( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 8 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	int8_t GetSigned8( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 8 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	uint8_t GetUnsigned8( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 16 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	int16_t GetSigned16( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 16 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	uint16_t GetUnsigned16( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 32 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	int32_t GetSigned32( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 32 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	uint32_t GetUnsigned32( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 64 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	int64_t GetSigned64( uint32_t uiColumn ) const;

	/**
	*	Gets a signed 64 bit integer.
	*	@param uiColumn Column to retrieve.
	*	@return An integer.
	*/
	uint64_t GetUnsigned64( uint32_t uiColumn ) const;

	/**
	*	Gets a 32 bit float.
	*	@param uiColumn Column to retrieve.
	*	@return A float.
	*/
	float GetFloat( uint32_t uiColumn ) const;

	/**
	*	Gets a 64 bit float.
	*	@param uiColumn Column to retrieve.
	*	@return A double.
	*/
	double GetDouble( uint32_t uiColumn ) const;

	/**
	*	Gets a string.
	*	@param uiColumn Column to retrieve.
	*	@return A string.
	*/
	std::string GetString( uint32_t uiColumn ) const;

	/**
	*	Gets a date instance.
	*	@param uiColumn Column to retrieve.
	*	@return A date instance.
	*/
	CASDateTime GetDate( uint32_t uiColumn ) const;

	/**
	*	Gets a time instance.
	*	@param uiColumn Column to retrieve.
	*	@return A time instance.
	*/
	CASTime GetTime( uint32_t uiColumn ) const;

	/**
	*	Gets a date time instance.
	*	@param uiColumn Column to retrieve.
	*	@return A date time instance.
	*/
	CASDateTime GetDateTime( uint32_t uiColumn ) const;

private:
	CASMySQLPreparedStatement* m_pStatement = nullptr;

	MYSQL_STMT* m_pMyStatement = nullptr;

	MYSQL_RES* m_pResultSet = nullptr;
	MYSQL_FIELD* m_pFields = nullptr;

	MYSQL_BIND* m_pBinds = nullptr;

	CASMySQLBind* m_pVariables = nullptr;

private:
	CASMySQLResultSet( const CASMySQLResultSet& ) = delete;
	CASMySQLResultSet& operator=( const CASMySQLResultSet& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H
