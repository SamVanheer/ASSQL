#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H

/**
*	Represents an SQL connection.
*/
class IASSQLConnection
{
public:
	virtual void AddRef() const = 0;
	virtual void Release() const = 0;

	/**
	*	@return Whether the connection is open.
	*/
	virtual bool IsOpen() const = 0;

	/**
	*	If the connection is open, closes the connection.
	*/
	virtual void Close() = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_ISQLCONNECTION_H