#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H

#include <atomic>
#include <mutex>
#include <string>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../CASSQLThreadPool.h"

class CASMySQLPreparedStatement;

/**
*	Represents a MySQL connection. This doesn't manage MySQL connections on its own; the connections are created in the query and statement execution methods.
*/
class CASMySQLConnection final : public CASAtomicRefCountedBaseClass
{
public:
	static const int DEFAULT_TIMEOUT = 60;

public:
	/**
	*	Constructor.
	*	@param pool Thread pool to use.
	*	@param pszHost Host IP address.
	*	@param pszUser User name to log in with.
	*	@param pszPassword Password to log in with.
	*	@param pszDatabase Database to default to.
	*	@param uiPort Port to connect to.
	*	@param pszUnixSocket If not null, unix socket to use.
	*	@param clientflag Client connection flags. CLIENT_MULTI_STATEMENTS is always set to maintain consistent behavior with the SQLite API.
	*/
	CASMySQLConnection( CASSQLThreadPool& pool,
						const char* const pszHost, const char* const pszUser, const char* const pszPassword, 
						const char* const pszDatabase, const unsigned int uiPort, const char* const pszUnixSocket,
						unsigned long clientflag );

	/**
	*	Destructor.
	*/
	~CASMySQLConnection();


	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	/**
	*	Opens a MySQL connection with the given settings.
	*	@return MySQL connection, or null if the connection failed to be created.
	*/
	MYSQL* Open();

	/**
	*	Closes a connection.
	*	@param pConnection Connection to close.
	*/
	void Close( MYSQL* pConnection );

	/**
	*	Executes a query.
	*	@param szQuery Query to execute.
	*	@param pCallback Optional. Callback to execute when the query has completed.
	*	@return Whether the query was successfully queued for execution.
	*/
	bool Query( const std::string& szQuery, asIScriptFunction* const pCallback = nullptr );

	/**
	*	Creates a prepared statement.
	*	@param szStatement Statement string. Must not be empty.
	*	@return Prepared statement, or null if the statement string was empty.
	*/
	CASMySQLPreparedStatement* CreatePreparedStatement( const std::string& szStatement );

	/**
	*	@return The thread pool used by this connection.
	*/
	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	/**
	*	@return The log function used by this connection.
	*/
	ASSQLLogFunction GetLogFunction() { return m_ThreadPool.GetLogFunction(); }

	/**
	*	@return The current timeout setting.
	*/
	int GetTimeoutTime() const { return m_iTimeout; }

	/**
	*	Sets the timeout setting.
	*	@param iTimeout Time until queries time out. Must be larger than 0.
	*/
	void SetTimeoutTime( const int iTimeout )
	{
		if( iTimeout > 0 )
			m_iTimeout = iTimeout;
	}

	/**
	*	@return The character set that is currently in use, or an empty string if the default is in use.
	*/
	std::string GetCharSet() const;

	/**
	*	Sets the character set to use.
	*	@param szCharSet Character set.
	*/
	void SetCharSet( const std::string& szCharSet );

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

	std::atomic<int> m_iTimeout = DEFAULT_TIMEOUT;

	//Mutex for mutable non-atomic connection data.
	mutable std::mutex m_DataMutex;

	std::string m_szCharSet;

private:
	CASMySQLConnection( const CASMySQLConnection& ) = delete;
	CASMySQLConnection& operator=( const CASMySQLConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLCONNECTION_H