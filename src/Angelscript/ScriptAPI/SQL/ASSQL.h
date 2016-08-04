#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H

class asIScriptEngine;

void RegisterScriptSQLConnection( asIScriptEngine& engine, const char* const pszObjectName );

void RegisterScriptSQL( asIScriptEngine& engine );

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_ASSQL_H