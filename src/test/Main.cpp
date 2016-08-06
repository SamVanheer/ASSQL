#include <iostream>

#include <Angelscript/CASManager.h>
#include <Angelscript/CASModule.h>
#include <Angelscript/IASInitializer.h>
#include <Angelscript/IASModuleBuilder.h>

#include <Angelscript/add_on/scriptbuilder.h>
#include <Angelscript/add_on/scriptstdstring.h>

#include <Angelscript/util/ASUtil.h>
#include <Angelscript/util/CASBaseClass.h>
#include <Angelscript/wrapper/ASCallable.h>

#include <Angelscript/ScriptAPI/SQL/ASSQL.h>
#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>
#include <Angelscript/ScriptAPI/SQL/IASSQLASyncItem.h>
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

class CASSQL final
{
public:
	CASSQL()
		: m_ThreadPool( std::thread::hardware_concurrency() )
	{
	}

	CASSQLThreadPool& GetThreadPool() { return m_ThreadPool; }

	CASSQLiteConnection* CreateSQLiteConnection( const std::string& szFilename )
	{
		//TODO: sanitize input filename.
		return new CASSQLiteConnection( m_ThreadPool, szFilename.c_str() );
	}

	CASMySQLConnection* CreateMySQLConnection( const std::string& szHost, const std::string& szUser, const std::string& szPassword )
	{
		//TODO: get all of this information from server config.
		return new CASMySQLConnection( m_ThreadPool, szHost.c_str(), szUser.c_str(), szPassword.c_str(), nullptr, 3306, nullptr, 0 );
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

		RegisterScriptSQL( engine );
		RegisterScriptSQLite( engine );
		RegisterScriptMySQL( engine );

		engine.RegisterObjectType( "CSQL", 0, asOBJ_REF | asOBJ_NOCOUNT );

		engine.RegisterObjectMethod( 
			"CSQL", "SQLiteConnection@ CreateSQLiteConnection(const string& in szFilename)", 
			asMETHOD( CASSQL, CreateSQLiteConnection ), asCALL_THISCALL );

		engine.RegisterObjectMethod(
			"CSQL", "MySQLConnection@ CreateMySQLConnection(const string& in szHost, const string& in szUser, const string& in szPassword)",
			asMETHOD( CASSQL, CreateMySQLConnection ), asCALL_THISCALL );

		engine.RegisterGlobalProperty( "CSQL SQL", &g_ASSQL );

		as::RegisterVarArgsFunction( engine, "void", "Print", "const string& in szFormat", 0, 8, asFUNCTION( Print ) );

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
		return builder.AddSectionFromFile( "../tests/test_SQLite.as" ) >= 0;
	}
};

int main( int iArgc, char* pszArgV[] )
{
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
				}
				while( bDidWork || g_ASSQL.GetThreadPool().ThreadsActive() );

				g_ASSQL.GetThreadPool().Stop( true );

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

	std::cin.get();

	return bSuccess ? EXIT_SUCCESS : EXIT_FAILURE;
}