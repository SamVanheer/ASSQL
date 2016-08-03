#include <iostream>

#include <Angelscript/util/CASBaseClass.h>

#include <Angelscript/ScriptAPI/SQL/ASSQL.h>
#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>
#include <Angelscript/ScriptAPI/SQL/IASSQLASyncItem.h>

#include "sqlite3.h"

class CASyncItem : public CASAtomicRefCountedBaseClass, public IASSQLASyncItem
{
public:

	void AddRef() const
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	void Execute()
	{
		std::cout << "Executing item" << std::endl;
	}
};

int main( int iArgc, char* pszArgV[] )
{
	CASSQLThreadPool pool( std::thread::hardware_concurrency() );

	auto pItem = new CASyncItem();

	pool.AddItem( pItem, nullptr );

	pItem->Release();

	std::cin.get();

	return 0;
}