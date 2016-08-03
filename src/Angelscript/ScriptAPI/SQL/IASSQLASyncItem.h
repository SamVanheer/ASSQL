#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H

class IASSQLASyncItem
{
public:

	virtual void AddRef() const = 0;

	virtual void Release() const = 0;

	virtual void Execute() = 0;

	virtual void CallbackInvoked() {}
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_IASSQLASYNCITEM_H