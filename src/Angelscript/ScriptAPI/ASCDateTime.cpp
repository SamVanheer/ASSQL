#include <angelscript.h>

#include "CASDateTime.h"

#include "CASDateTime.h"

static void DefaultConstructCDateTime( CASDateTime* pTime )
{
	new( pTime ) CASDateTime();
}

static void ConstructCDateTime( CASDateTime* pTime, uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay,
								uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	new( pTime ) CASDateTime( uiYear, uiMonth, uiDay, uiHour, uiMinute, uiSecond, uiMillisecond );
}

static void StringConstructCDateTime( CASDateTime* pTime, const std::string& szString )
{
	new( pTime ) CASDateTime( szString );
}

static void CopyConstructCDateTime( CASDateTime* pTime, const CASDateTime& other )
{
	new( pTime ) CASDateTime( other );
}

static void DestructCDateTime( CASDateTime* pTime )
{
	pTime->~CASDateTime();
}

void RegisterScriptCDateTime( asIScriptEngine& engine )
{
	const char* const pszObjectName = "CDateTime";

	engine.RegisterObjectType(
		pszObjectName, sizeof( CASDateTime ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CDateTime()",
		asFUNCTION( DefaultConstructCDateTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CDateTime(uint16 uiYear, uint8 uiMonth = 1, uint8 uiDay = 1, "
			"uint8 uiHour = 0, uint8 uiMinute = 0, uint8 uiSecond = 0, uint16 uiMillisecond = 0 )",
		asFUNCTION( ConstructCDateTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CDateTime(const CDateTime& in other)",
		asFUNCTION( CopyConstructCDateTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_CONSTRUCT, "void CDateTime(const string& in szString)",
		asFUNCTION( StringConstructCDateTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName,
		asBEHAVE_DESTRUCT, "void CDateTime()",
		asFUNCTION( DestructCDateTime ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CDateTime& opAssign(const CDateTime& in other)",
		asMETHOD( CASDateTime, operator= ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetYear() const",
		asMETHOD( CASDateTime, GetYear ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetMonth() const",
		asMETHOD( CASDateTime, GetMonth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetDay() const",
		asMETHOD( CASDateTime, GetDay ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetDayOfYear() const",
		asMETHODPR( CASDateTime, GetDayOfYear, () const, uint16_t ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetHour() const",
		asMETHOD( CASDateTime, GetHour ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetMinute() const",
		asMETHOD( CASDateTime, GetMinute ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint8 GetSecond() const",
		asMETHOD( CASDateTime, GetSecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "uint16 GetMillisecond() const",
		asMETHOD( CASDateTime, GetMillisecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetYear(uint16 uiYear)",
		asMETHOD( CASDateTime, SetYear ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetMonth(uint8 uiMonth)",
		asMETHOD( CASDateTime, SetMonth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetDay(uint8 uiDay)",
		asMETHOD( CASDateTime, SetDay ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetDayOfYear(uint8 uiDayOfYear)",
		asMETHOD( CASDateTime, SetDayOfYear ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetHour(uint8 uiHour)",
		asMETHOD( CASDateTime, SetHour ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetMinute(uint8 uiMinute)",
		asMETHOD( CASDateTime, SetMinute ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetSecond(uint8 uiSecond)",
		asMETHOD( CASDateTime, SetSecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool SetMillisecond(uint8 uiMillisecond)",
		asMETHOD( CASDateTime, SetMillisecond ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Set(uint16 uiYear, uint8 uiMonth, uint8 uiDay)",
		asMETHODPR( CASDateTime, Set, ( uint16_t, uint8_t, uint8_t ), bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Set(uint16 uiYear, uint8 uiMonth, uint8 uiDay,"
		"uint8 uiHour, uint8 uiMinute, uint8 uiSecond, uint16 uiMillisecond )",
		asMETHODPR( CASDateTime, Set, ( uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t ), bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Set(const string& in szString)",
		asMETHODPR( CASDateTime, Set, ( const std::string& ), bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddYears(int32 iHours)",
		asMETHOD( CASDateTime, AddYears ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddMonths(int32 iHours)",
		asMETHOD( CASDateTime, AddMonths ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddDays(int32 iHours)",
		asMETHOD( CASDateTime, AddDays ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddHours(int32 iHours)",
		asMETHOD( CASDateTime, AddHours ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddMinutes(int32 iMinutes)",
		asMETHOD( CASDateTime, AddMinutes ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddSeconds(int32 iSeconds)",
		asMETHOD( CASDateTime, AddSeconds ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool AddMilliseconds(int32 iMilliseconds)",
		asMETHOD( CASDateTime, AddMilliseconds ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "double TimeDifference(const CDateTime& in time) const",
		asMETHOD( CASDateTime, TimeDifference ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CDateTime GetDate() const",
		asMETHOD( CASDateTime, GetDate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CTime GetTime() const",
		asMETHOD( CASDateTime, GetTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string StrTime(const bool bWithMillisecond = false) const",
		asMETHOD( CASDateTime, StrTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string StrDate() const",
		asMETHOD( CASDateTime, StrDate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int opCmp(const CDateTime& in time) const",
		asMETHOD( CASDateTime, Compare ), asCALL_THISCALL );

	//Operators <, <=, >, >=, == and != are all implemented automatically by the opCmp method implementation. - Solokiller

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "DateTime" );

	engine.RegisterGlobalFunction(
		"CDateTime Now()",
		asFUNCTION( CASDateTime::Now ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CDateTime NowUTC()",
		asFUNCTION( CASDateTime::NowUTC ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"bool IsLeapYear(uint16 uiYear)",
		asFUNCTION( CASDateTime::IsLeapYear ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint16 GetDaysInYear(uint16 uiYear)",
		asFUNCTION( CASDateTime::GetDaysInYear ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint8 GetDaysInMonth(uint16 uiYear, uint8 uiMonth)",
		asFUNCTION( CASDateTime::GetDaysInMonth ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"bool IsValidDate(uint16 uiYear, uint8 uiMonth, uint8 uiDay)",
		asFUNCTION( CASDateTime::IsValidDate ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint16 GetDayOfYear(uint16 uiYear, uint8 uiMonth, uint8 uiDay)",
		asFUNCTIONPR( CASDateTime::GetDayOfYear, ( uint16_t, uint8_t, uint8_t ), uint16_t ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CDateTime GetReverseDayOfYear(uint16 uiYear, uint16 uiDayOfYear)",
		asFUNCTION( CASDateTime::GetReverseDayOfYear ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}