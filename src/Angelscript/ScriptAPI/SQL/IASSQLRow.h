#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H

#include <string>

#include "ASSQLDataType.h"

#include "IASSQLASyncItem.h"

class IASSQLRow : public IASSQLASyncItem
{
public:

	/**
	*	@return The row index.
	*/
	virtual int GetRowIndex() const = 0;

	/**
	*	@return The column count.
	*/
	virtual int GetColumnCount() const = 0;

	/**
	*	@return The type of the given column. Invalid column indices are always null.
	*/
	virtual ASSQLDataType GetColumnType( const int iColumn ) const = 0;

	/**
	*	@return Whether the given column is null. Invalid column indices are always null.
	*/
	virtual bool IsColumnNull( const int iColumn ) const = 0;

	/**
	*	@return A column integer.
	*/
	virtual int GetColumnInt( int iColumn ) const = 0;

	/**
	*	@return A column double.
	*/
	virtual double GetColumnDouble( int iColumn ) const = 0;

	/**
	*	@return A column string.
	*/
	virtual std::string GetColumnString( int iColumn ) const = 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLROW_H