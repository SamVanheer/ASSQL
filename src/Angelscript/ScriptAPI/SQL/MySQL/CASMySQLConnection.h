#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLConnection.h"

class CASSQLThreadPool;

class CASMySQLConnection final : public IASSQLConnection, public CASAtomicRefCountedBaseClass
{
public:
	CASMySQLConnection( CASSQLThreadPool& pool, 
						const char* const pszHost, const char* const pszUser, const char* const pszPassword, 
						const char* const pszDatabase, const unsigned int uiPort, const char* const pszUnixSocket,
						unsigned long clientflag );
	~CASMySQLConnection();

	void AddRef() const override final
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const override final
	{
		if( InternalRelease() )
			delete this;
	}

	bool IsOpen() const override;

	void Close() override;

	bool Query( const std::string& szQuery, asIScriptFunction* const pCallback = nullptr ) override;

	IASSQLPreparedStatement* CreatePreparedStatement( const std::string& szStatement ) override;

	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	MYSQL* GetConnection() { return m_pConnection; }

private:
	CASSQLThreadPool& m_ThreadPool;

	MYSQL* m_pConnection = nullptr;

private:
	CASMySQLConnection( const CASMySQLConnection& ) = delete;
	CASMySQLConnection& operator=( const CASMySQLConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H