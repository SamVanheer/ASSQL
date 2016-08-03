#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H

#include <sqlite3.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLConnection.h"

/**
*	Angelscript SQLite connection.
*/
class CASSQLiteConnection : public IASSQLConnection, public CASAtomicRefCountedBaseClass
{
public:
	CASSQLiteConnection( const char* const pszFilename );

	/**
	*	Destructor.
	*	Closes the connection if it is still open.
	*/
	~CASSQLiteConnection();

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

private:
	sqlite3* m_pConnection = nullptr;

private:
	CASSQLiteConnection( const CASSQLiteConnection& ) = delete;
	CASSQLiteConnection& operator=( const CASSQLiteConnection& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_SQLITE_CASSQLITECONNECTION_H