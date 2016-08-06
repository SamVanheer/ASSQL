#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLRESULTSET_H

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncItem.h"

class CASMySQLBind;
class CASMySQLPreparedStatement;

//TODO: IASSQLASyncItem isn't the right base class for this. - Solokiller
class CASMySQLResultSet final : public IASSQLASyncItem, public CASAtomicRefCountedBaseClass
{
public:
	CASMySQLResultSet( CASMySQLPreparedStatement* pStatement );
	~CASMySQLResultSet();

	void AddRef() const
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	void Execute() override {}

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
	*	Gets a signed 32 bit integer.
	*/
	int32_t GetSigned32( int iColumn ) const;

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
