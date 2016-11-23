#include <cstdarg>
#include <iostream>

#include <mysql.h>

#include <Angelscript/CASManager.h>
#include <Angelscript/CASModule.h>
#include <Angelscript/IASInitializer.h>
#include <Angelscript/IASModuleBuilder.h>

#include <Angelscript/add_on/scriptbuilder.h>
#include <Angelscript/add_on/scriptstdstring.h>

#include <Angelscript/util/ASUtil.h>
#include <Angelscript/util/CASBaseClass.h>
#include <Angelscript/wrapper/ASCallable.h>

#include <Angelscript/ScriptAPI/ASCDateTime.h>
#include <Angelscript/ScriptAPI/ASCTime.h>
#include <Angelscript/ScriptAPI/SQL/ASSQL.h>
#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>
#include <Angelscript/ScriptAPI/SQL/SQLite/ASSQLite.h>
#include <Angelscript/ScriptAPI/SQL/SQLite/CASSQLiteConnection.h>
#include <Angelscript/ScriptAPI/SQL/MySQL/ASMySQL.h>
#include <Angelscript/ScriptAPI/SQL/MySQL/CASMySQLConnection.h>

void ASMessageCallback( asSMessageInfo* pMsg )
{
	const char* pszType = "";

	switch( pMsg->type )
	{
	case asMSGTYPE_ERROR:
		pszType = "Error: ";
		break;

	case asMSGTYPE_WARNING:
		pszType = "Warning: ";
		break;
	}

	std::cout << pszType << pMsg->section << '(' << pMsg->row << ", " << pMsg->col << ')' << std::endl << pMsg->message << std::endl;
}

void SQLLogFunction( const char* const pszFormat, ... )
{
	assert( pszFormat );

	char szBuffer[ 8192 ];

	va_list list;

	va_start( list, pszFormat );
	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );
	va_end( list );

	if( iResult >= 0 && static_cast<size_t>( iResult ) < sizeof( szBuffer ) )
	{
		std::cout << szBuffer;
	}
}

class CASSQL final
{
public:
	CASSQL()
		: m_ThreadPool( 1, &::SQLLogFunction )
	{
	}

	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	CASSQLiteConnection* CreateSQLiteConnection( const std::string& szFilename )
	{
		//TODO: sanitize input filename.
		return new CASSQLiteConnection( m_ThreadPool, szFilename.c_str() );
	}

	CASMySQLConnection* CreateMySQLConnection( const std::string& szHost, const std::string& szUser, const std::string& szPassword, const std::string& szDatabase = "" )
	{
		//TODO: get all of this information from server config.
		return new CASMySQLConnection( m_ThreadPool, szHost.c_str(), szUser.c_str(), szPassword.c_str(), szDatabase.c_str(), 3306, "", 0 );
	}

private:
	CASSQLThreadPool m_ThreadPool;
};

CASSQL g_ASSQL;

void Print( asIScriptGeneric* pArguments )
{
	char szBuffer[ 4096 ];

	const std::string& szFormat = *reinterpret_cast<const std::string*>( pArguments->GetArgAddress( 0 ) );

	if( as::SPrintf( szBuffer, szFormat.c_str(), 1, *pArguments ) )
	{
		std::cout << szBuffer;
	}
}

void ScriptAssert( const bool bCondition )
{
	if( bCondition )
		return;

	std::cout << "Assertion failed: ";

	if( asIScriptContext* pContext = asGetActiveContext() )
	{
		if( auto pFunction = pContext->GetFunction() )
		{
			std::cout << "Section \"" << pFunction->GetScriptSectionName() << "\": " << pFunction->GetNamespace() << "::" << pFunction->GetName() << '(';

			std::cout << ')';
		}
		else
			std::cout << "Unknown section and function";

		int iColumn;

		const int iLineNumber = pContext->GetLineNumber( 0, &iColumn );

		std::cout << std::endl << "Line " << iLineNumber << ", Column " << iColumn;
	}
	else
		std::cout << "Unknown condition";

	std::cout << std::endl;
}

class CASInitializer final : public IASInitializer
{
public:
	bool GetMessageCallback( asSFuncPtr& outFuncPtr, void*& pOutObj, asDWORD& outCallConv ) override
	{
		outFuncPtr = asFUNCTION( ASMessageCallback );
		pOutObj = nullptr;
		outCallConv = asCALL_CDECL;

		return true;
	}

	bool RegisterCoreAPI( CASManager& manager ) override
	{
		auto& engine = *manager.GetEngine();

		RegisterStdString( &engine );

		RegisterScriptCTime( engine );
		RegisterScriptCDateTime( engine );
		RegisterScriptSQLite( engine );
		RegisterScriptMySQL( engine );

		RegisterScriptEventAPI( engine );

		engine.RegisterObjectType( "CSQL", 0, asOBJ_REF | asOBJ_NOCOUNT );

		engine.RegisterObjectMethod( 
			"CSQL", "SQLiteConnection@ CreateSQLiteConnection(const " AS_STRING_OBJNAME "& in szFilename)",
			asMETHOD( CASSQL, CreateSQLiteConnection ), asCALL_THISCALL );

		engine.RegisterObjectMethod(
			"CSQL", "MySQLConnection@ CreateMySQLConnection("
			"const " AS_STRING_OBJNAME "& in szHost, const " AS_STRING_OBJNAME "& in szUser,"
			"const " AS_STRING_OBJNAME "& in szPassword, const " AS_STRING_OBJNAME "& in szDatabase = \"\")",
			asMETHOD( CASSQL, CreateMySQLConnection ), asCALL_THISCALL );

		engine.RegisterGlobalProperty( "CSQL SQL", &g_ASSQL );

		as::RegisterVarArgsFunction( 
			engine, "void", "Print", "const " AS_STRING_OBJNAME "& in szFormat",
			0, 8, asFUNCTION( Print ) );

		engine.RegisterGlobalFunction( 
			"void Assert(const bool bCondition)", 
			asFUNCTION( ScriptAssert ), asCALL_CDECL );

		return true;
	}

	bool RegisterAPI( CASManager& manager )
	{
		return true;
	}
};

class CASModuleBuilder : public IASModuleBuilder
{
public:
	bool AddScripts( CScriptBuilder& builder ) override
	{
		//Assumes the working directory is <repo>/working_dir
		return builder.AddSectionFromFile( "../tests/test_MySQL.as" ) >= 0;
	}
};

int main( int iArgc, char* pszArgV[] )
{
	if( mysql_library_init( 0, nullptr, nullptr ) )
	{
		std::cout << "Failed to initialize MySQL" << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}

	CASManager manager;

	bool bSuccess = false;

	{
		CASInitializer initializer;

		bSuccess = manager.Initialize( initializer );
	}

	if( bSuccess )
	{
		std::cout << "Starting tests" << std::endl;

		manager.GetModuleManager().AddDescriptor( "Test", 0xFFFFFFFF, as::ModulePriority::HIGHEST );

		CASModuleBuilder builder;

		if( CASModule* pModule = manager.GetModuleManager().BuildModule( "Test", "Test", builder ) )
		{
			if( auto pFunction = pModule->GetModule()->GetFunctionByDecl( "void main()" ) )
			{
				as::Call( pFunction );

				CASOwningContext ctx( *manager.GetEngine() );

				bool bDidWork = false;

				do
				{
					bDidWork = g_ASSQL.GetThreadPool().ProcessQueue( *ctx.GetContext() );

					std::this_thread::yield();
				}
				while( bDidWork || g_ASSQL.GetThreadPool().ThreadsActive() || g_ASSQL.GetThreadPool().GetThreadQueue().GetQueueSize() > 0 );

				g_ASSQL.GetThreadPool().Stop( true );

				g_ASSQL.GetThreadPool().GetThreadQueue().Clear();

				std::cout << g_ASSQL.GetThreadPool().GetThreadQueue().GetQueueSize() << " items left in the queue" << std::endl;
			}
			else
			{
				std::cout << "Couldn't find main function" << std::endl;
			}
		}
		else
		{
			std::cout << "Couldn't build module" << std::endl;
		}
	}

	manager.Shutdown();

	std::cout << "Finished running tests" << std::endl;

	mysql_library_end();

	std::cin.get();

	return bSuccess ? EXIT_SUCCESS : EXIT_FAILURE;
}