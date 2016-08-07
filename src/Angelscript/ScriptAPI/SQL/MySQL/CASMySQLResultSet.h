#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H

#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncItem.h"

class CASMySQLBind;
class CASMySQLPreparedStatement;

class CASMySQLResultSet final : public IASSQLASyncItem, public CASAtomicRefCountedBaseClass
{
public:
	CASMySQLResultSet( CASMySQLPreparedStatement* pStatement );
	~CASMySQLResultSet();

private:
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

	/**
	*	@return Whether this result set is valid.
	*/
	bool IsValid() const;

	/**
	*	@return The number of fields in a single row.
	*/
	int GetFieldCount() const;

	/**
	*	Fetches the next row.
	*	@return Whether there is a row to process, false if no data is left.
	*/
	bool Next();

	/**
	*	@return Whether the given column is null.
	*/
	bool IsNull( int iColumn ) const;

	/**
	*	@param iColumn Column to retrieve.
	*	@return A boolean.
	*/
	bool GetBoolean( int iColumn ) const;

	/**
	*	Gets a signed 8 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	int8_t GetSigned8( int iColumn ) const;

	/**
	*	Gets a signed 8 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	uint8_t GetUnsigned8( int iColumn ) const;

	/**
	*	Gets a signed 16 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	int16_t GetSigned16( int iColumn ) const;

	/**
	*	Gets a signed 16 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	uint16_t GetUnsigned16( int iColumn ) const;

	/**
	*	Gets a signed 32 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	int32_t GetSigned32( int iColumn ) const;

	/**
	*	Gets a signed 32 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	uint32_t GetUnsigned32( int iColumn ) const;

	/**
	*	Gets a signed 64 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	int64_t GetSigned64( int iColumn ) const;

	/**
	*	Gets a signed 64 bit integer.
	*	@param iColumn Column to retrieve.
	*	@return An integer.
	*/
	uint64_t GetUnsigned64( int iColumn ) const;

	/**
	*	Gets a 32 bit float.
	*	@param iColumn Column to retrieve.
	*	@return A float.
	*/
	float GetFloat( int iColumn ) const;

	/**
	*	Gets a 64 bit float.
	*	@param iColumn Column to retrieve.
	*	@return A double.
	*/
	double GetDouble( int iColumn ) const;

	/**
	*	Gets a string.
	*	@param iColumn Column to retrieve.
	*	@return A string.
	*/
	std::string GetString( int iColumn ) const;

private:
	CASMySQLPreparedStatement* m_pStatement = nullptr;

	MYSQL_RES* m_pResultSet = nullptr;
	MYSQL_FIELD* m_pFields = nullptr;

	MYSQL_BIND* m_pBinds = nullptr;

	CASMySQLBind* m_pVariables = nullptr;

private:
	CASMySQLResultSet( const CASMySQLResultSet& ) = delete;
	CASMySQLResultSet& operator=( const CASMySQLResultSet& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H
