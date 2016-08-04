#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H

#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLQuery.h"

class CASMySQLConnection;

class CASMySQLQuery final : public IASSQLQuery, public CASAtomicRefCountedBaseClass
{
public:
	CASMySQLQuery( CASMySQLConnection* pConnection, const char* const pszQuery );
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

	bool IsValid() const override;

	CASMySQLConnection* GetConnection() { return m_pConnection; }

private:
	CASMySQLConnection* m_pConnection;
	std::string m_szQuery;

private:
	CASMySQLQuery( const CASMySQLQuery& ) = delete;
	CASMySQLQuery& operator=( const CASMySQLQuery& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLQUERY_H