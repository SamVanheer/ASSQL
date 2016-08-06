#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../CASSQLThreadPool.h"

#include "../IASSQLConnection.h"

class CASMySQLPreparedStatement;

class CASMySQLConnection final : public IASSQLConnection, public CASAtomicRefCountedBaseClass
{
public:
	/**
	*	Constructor.
	*	@param clientflag Client connection flags. CLIENT_MULTI_STATEMENTS is always set to maintain consistent behavior with the SQLite API.
	*/
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

	CASMySQLPreparedStatement* CreatePreparedStatement( const std::string& szStatement );

	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	ASSQLLogFunction GetLogFunction() { return m_ThreadPool.GetLogFunction(); }

	MYSQL* GetConnection() { return m_pConnection; }

private:
	CASSQLThreadPool& m_ThreadPool;

	MYSQL* m_pConnection = nullptr;

private:
	CASMySQLConnection( const CASMySQLConnection& ) = delete;
	CASMySQLConnection& operator=( const CASMySQLConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H