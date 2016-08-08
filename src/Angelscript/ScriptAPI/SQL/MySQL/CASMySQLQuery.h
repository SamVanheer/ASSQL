#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H

#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncCommand.h"

class CASMySQLConnection;

/**
*	MySQL query. Executes asynchronously.
*/
class CASMySQLQuery final : public IASSQLASyncCommand, public CASAtomicRefCountedBaseClass
{
public:
	/**
	*	Constructor.
	*	@param pConnection The connection that created this query.
	*	@param pszQuery Query string.
	*/
	CASMySQLQuery( CASMySQLConnection* pConnection, const char* const pszQuery );

	/**
	*	Destructor.
	*/
	~CASMySQLQuery();

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

	/**
	*	@return The connection that created this query.
	*/
	CASMySQLConnection* GetConnection() { return m_pConnection; }

private:
	CASMySQLConnection* m_pConnection;
	std::string m_szQuery;

private:
	CASMySQLQuery( const CASMySQLQuery& ) = delete;
	CASMySQLQuery& operator=( const CASMySQLQuery& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H