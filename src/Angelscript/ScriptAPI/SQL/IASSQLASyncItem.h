#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H

/**
*	An item that needs to be passed to an Angelscript callback.
*/
class IASSQLASyncItem
{
public:

	virtual void AddRef() const = 0;

	virtual void Release() const = 0;

	virtual void CallbackInvoked() {}
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H