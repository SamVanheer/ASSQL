#include <new>
#include <string>

#include <angelscript.h>

#include "CASTime.h"

#include "ASCTime.h"

static void DefaultConstructCTime( CASTime* pTime )
{
	new( pTime ) CASTime();
}

static void ConstructCTime( CASTime* pTime, uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	new( pTime ) CASTime( uiHour, uiMinute, uiSecond, uiMillisecond );
}

static void StringConstructCTime( CASTime* pTime, const std::string& szString )
{
	new( pTime ) CASTime( szString );
}

static void CopyConstructCTime( CASTime* pTime, const CASTime& other )
{
	new( pTime ) CASTime( other );
}

static void DestructCTime( CASTime* pTime )
{
	pTime->~CASTime();
}

void RegisterScriptCTime( asIScriptEngine& engine )
{
	const char* const pszObjectName = "CTime";

	engine.RegisterObjectType(
		pszObjectName, sizeof( CASTime ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK );

	engine.RegisterObjectBehaviour(
		pszObjectName, 
		asBEHAVE_CONSTRUCT, "void CTime()",
		asFUNCTION( DefaultConstructCTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CTime(uint8 uiHour, uint8 uiMinute = 0, uint8 uiSecond = 0, uint16 uiMillisecond = 0)",
		asFUNCTION( ConstructCTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CTime(const CTime& in other)",
		asFUNCTION( CopyConstructCTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CTime(const string& in szString)",
		asFUNCTION( StringConstructCTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_DESTRUCT, "void CTime()",
		asFUNCTION( DestructCTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CTime& opAssign(const CTime& in other)",
		asMETHOD( CASTime, operator= ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetHour() const",
		asMETHOD( CASTime, GetHour ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetMinute() const",
		asMETHOD( CASTime, GetMinute ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetSecond() const",
		asMETHOD( CASTime, GetSecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetMillisecond() const",
		asMETHOD( CASTime, GetMillisecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetHour(uint8 uiHour)",
		asMETHOD( CASTime, SetHour ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetMinute(uint8 uiMinute)",
		asMETHOD( CASTime, SetMinute ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetSecond(uint8 uiSecond)",
		asMETHOD( CASTime, SetSecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetMillisecond(uint8 uiMillisecond)",
		asMETHOD( CASTime, SetMillisecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Set(uint8 uiHour, uint8 uiMinute, uint8 uiSecond, uint16 uiMilliseconds)",
		asMETHODPR( CASTime, Set, ( uint8_t, uint8_t, uint8_t, uint16_t ), bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Set(const string& in szString)",
		asMETHODPR( CASTime, Set, ( const std::string& ), bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddHours(int32 iHours)",
		asMETHOD( CASTime, AddHours ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddMinutes(int32 iMinutes)",
		asMETHOD( CASTime, AddMinutes ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddSeconds(int32 iSeconds)",
		asMETHOD( CASTime, AddSeconds ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddMilliseconds(int32 iMilliseconds)",
		asMETHOD( CASTime, AddMilliseconds ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "double TimeDifference(const CTime& in time) const",
		asMETHOD( CASTime, TimeDifference ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string StrTime(const bool bWithMillisecond = false) const",
		asMETHOD( CASTime, StrTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int opCmp(const CTime& in time) const",
		asMETHOD( CASTime, Compare ), asCALL_THISCALL );

	//Operators <, <=, >, >=, == and != are all implemented automatically by the opCmp method implementation. - Solokiller

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Time" );

	engine.RegisterGlobalFunction(
		"CTime Now()",
		asFUNCTION( CASTime::Now ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CTime NowUTC()",
		asFUNCTION( CASTime::NowUTC ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}