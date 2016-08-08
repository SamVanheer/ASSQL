#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H

#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../CASSQLThreadPool.h"

class CASMySQLPreparedStatement;

class CASMySQLConnection final : public CASAtomicRefCountedBaseClass
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


	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	MYSQL* Open();

	void Close( MYSQL* pConnection );

	bool Query( const std::string& szQuery, asIScriptFunction* const pCallback = nullptr );

	CASMySQLPreparedStatement* CreatePreparedStatement( const std::string& szStatement );

	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	ASSQLLogFunction GetLogFunction() { return m_ThreadPool.GetLogFunction(); }

private:
	CASSQLThreadPool& m_ThreadPool;

	//TODO: encrypt this if possible - Solokiller
	const std::string m_szHost;
	const std::string m_szUser;
	const std::string m_szPass;
	const std::string m_szDatabase;

	const unsigned int m_uiPort;

	const std::string m_szUnixSocket;

	const unsigned long m_uiClientFlag;

private:
	CASMySQLConnection( const CASMySQLConnection& ) = delete;
	CASMySQLConnection& operator=( const CASMySQLConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H