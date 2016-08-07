#include "CASDateTime.h"

namespace
{
static const uint8_t g_uiMonthLengths[ 12 ] = 
{
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
}

CASDateTime::CASDateTime( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay,
			 uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	Set( uiYear, uiMonth, uiDay, uiHour, uiMinute, uiSecond, uiMillisecond );
}

CASDateTime::CASDateTime( const tm& time )
{
	Set( time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, 0 );
}

CASDateTime::CASDateTime( const time_t& time, const bool bUseLocalTime )
	: CASDateTime( *( bUseLocalTime ? localtime : gmtime )( &time ) )
{
}

CASDateTime::CASDateTime( const std::string& szString )
{
	Set( szString );
}

uint16_t CASDateTime::GetDayOfYear() const
{
	return GetDayOfYear( GetYear(), GetMonth(), GetDay() );
}

bool CASDateTime::SetYear( uint16_t uiYear )
{
	if( uiYear == 0 )
		return false;

	m_uiYear = uiYear;

	if( !IsValidDate( uiYear, GetMonth(), GetDay() ) )
	{
		m_uiMonth = 1;
		m_uiDay = 1;
	}

	return true;
}

bool CASDateTime::SetMonth( uint8_t uiMonth )
{
	if( uiMonth == 0 || uiMonth > 12 )
		return false;

	m_uiMonth = uiMonth;

	const uint8_t uiDaysInMonth = GetDaysInMonth( GetYear(), uiMonth );

	if( GetDay() > uiDaysInMonth )
		m_uiDay = 1;

	return true;
}

bool CASDateTime::SetDay( uint8_t uiDay )
{
	const uint8_t uiDaysInMonth = GetDaysInMonth( GetYear(), GetMonth() );

	if( uiDay == 0 || uiDay > uiDaysInMonth )
		return false;

	m_uiDay = uiDay;

	return true;
}

bool CASDateTime::SetDayOfYear( uint16_t uiDayOfYear )
{
	CASDateTime time = GetReverseDayOfYear( GetYear(), uiDayOfYear );

	SetMonth( time.GetMonth() );
	SetDay( time.GetDay() );

	return true;
}

bool CASDateTime::Set( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay )
{
	if( !IsValidDate( uiYear, uiMonth, uiDay ) )
		return false;

	m_uiYear = uiYear;
	m_uiMonth = uiMonth;
	m_uiDay = uiDay;

	return true;
}

bool CASDateTime::Set( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay,
		  uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	if( !IsValidDate( uiYear, uiMonth, uiDay ) ||
		uiHour >= 24 || uiMinute >= 60 || uiSecond >= 60 || uiMillisecond >= 1000 )
		return false;

	m_uiYear = uiYear;
	m_uiMonth = uiMonth;
	m_uiDay = uiDay;

	return CASTime::Set( uiHour, uiMinute, uiSecond, uiMillisecond );
}

bool CASDateTime::Set( const std::string& szString )
{
	if( szString.empty() )
		return false;

	const size_t uiLength = szString.length();

	if( uiLength < 10 )
		return false;

	uint16_t uiYear = static_cast<uint16_t>( std::stoul( szString.substr( 0, 4 ) ) );
	uint8_t uiMonth = static_cast<uint8_t>( std::stoul( szString.substr( 5, 2 ) ) );
	uint8_t uiDay = static_cast<uint8_t>( std::stoul( szString.substr( 8, 2 ) ) );

	if( uiLength >= 13 )
		CASTime::Set( szString.substr( 11 ) );

	return Set( uiYear, uiMonth, uiDay, GetHour(), GetMinute(), GetSecond(), GetMillisecond() );
}

bool CASDateTime::AddYears( int32_t iYears )
{
	if( iYears == 0 )
		return true;

	return SetYear( GetYear() + iYears );
}

bool CASDateTime::AddMonths( int32_t iMonths )
{
	if( iMonths == 0 )
		return true;

	int32_t iYears = iMonths / 12;

	//Make the current month zero based.
	iMonths = ( iMonths % 12 ) + ( GetMonth() - 1 );

	if( iMonths >= 12 )
	{
		++iYears;
		iMonths %= 12;
	}
	else if( iMonths < 0 )
	{
		--iYears;
		iMonths = 12 - iMonths;
	}

	CASDateTime temp( *this );

	if( iYears )
	{
		if( !temp.AddYears( iYears ) )
			return false;
	}

	//Make the month one based again.
	if( !temp.SetMonth( iMonths + 1 ) )
		return false;

	*this = temp;

	return true;
}

bool CASDateTime::AddDays( int32_t iDays )
{
	if( iDays == 0 )
		return true;

	int32_t iDir = iDays > 0 ? 1 : -1;

	int32_t iYear = GetYear();

	int32_t iDaysIn = GetDaysInYear( iYear );

	int32_t iDaysOf = GetDayOfYear() + iDays;

	while( iDaysOf > iDaysIn || iDaysOf < 0 )
	{
		iDaysOf -= iDaysIn * iDir;
		iYear += iDir;
		iDaysIn += GetDaysInYear( iYear );
	}

	CASDateTime temp = *this;

	temp.SetYear( iYear );

	if( !temp.SetDayOfYear( iDaysOf ) )
		return false;

	*this = temp;

	return true;
}

bool CASDateTime::AddHours( int32_t iHours )
{
	if( iHours == 0 )
		return true;

	int32_t iDays = iHours / 24;

	iHours = ( iHours % 24 ) + GetHour();

	if( iHours >= 24 )
	{
		++iDays;
		iHours %= 24;
	}
	else if( iHours < 0 )
	{
		--iDays;
		iHours = 24 - iHours;
	}

	CASDateTime temp( *this );

	if( !temp.AddDays( iDays ) )
		return false;

	if( !temp.SetHour( iHours ) )
		return false;

	*this = temp;

	return true;
}

bool CASDateTime::AddMinutes( int32_t iMinutes )
{
	if( iMinutes == 0 )
		return true;

	int32_t iHours = iMinutes / 60;

	iMinutes = ( iMinutes % 60 ) + GetMinute();

	if( iMinutes >= 60 )
	{
		++iHours;
		iMinutes %= 60;
	}
	else if( iMinutes < 0 )
	{
		--iHours;
		iMinutes = 60 - iMinutes;
	}

	CASDateTime temp( *this );

	if( !temp.AddHours( iHours ) )
		return false;

	if( !temp.SetMinute( iMinutes ) )
		return false;

	*this = temp;

	return true;
}

bool CASDateTime::AddSeconds( int32_t iSeconds )
{
	if( iSeconds == 0 )
		return true;

	int32_t iMinutes = iSeconds / 60;

	iSeconds = ( iSeconds % 60 ) + GetSecond();

	if( iSeconds >= 60 )
	{
		++iMinutes;
		iSeconds %= 60;
	}
	else if( iSeconds < 0 )
	{
		--iMinutes;
		iSeconds = 60 - iSeconds;
	}

	CASDateTime temp( *this );

	if( !temp.AddMinutes( iMinutes ) )
		return false;

	if( !temp.SetSecond( iSeconds ) )
		return false;

	*this = temp;

	return true;
}

bool CASDateTime::AddMilliseconds( int32_t iMilliseconds )
{
	if( iMilliseconds == 0 )
		return true;

	int32_t iSeconds = iMilliseconds / 1000;

	iMilliseconds = ( iMilliseconds % 1000 ) + GetMillisecond();

	if( iMilliseconds >= 1000 )
	{
		++iSeconds;
		iMilliseconds %= 1000;
	}
	else if( iMilliseconds < 0 )
	{
		--iSeconds;
		iMilliseconds = 1000 - iMilliseconds;
	}

	CASDateTime temp( *this );

	if( !temp.AddSeconds( iSeconds ) )
		return false;

	if( !temp.SetMillisecond( iMilliseconds ) )
		return false;

	*this = temp;

	return true;
}

tm CASDateTime::Totm() const
{
	tm time;

	time.tm_year = GetYear() - 1900;
	time.tm_mon = GetMonth() - 1;
	time.tm_mday = GetDay();
	time.tm_hour = GetHour();
	time.tm_min = GetMinute();
	time.tm_sec = GetSecond();

	return time;
}

time_t CASDateTime::MkTime() const
{
	tm time = Totm();

	return mktime( &time );
}

double CASDateTime::TimeDifference( const CASDateTime& time ) const
{
	return ::difftime( MkTime(), time.MkTime() );
}

CASDateTime CASDateTime::GetDate() const
{
	return CASDateTime( GetYear(), GetMonth(), GetDay() );
}

CASTime CASDateTime::GetTime() const
{
	return *this;
}

std::string CASDateTime::StrTime( const bool bWithMillisecond ) const
{
	char buffer[ 32 ];

	if( bWithMillisecond && GetMillisecond() != 0 )
	{
		sprintf( buffer, "%04u-%02u-%02u %02u:%02u:%02u.%03u", 
				 GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond(), GetMillisecond() );
	}
	else
	{
		sprintf( buffer, "%04u-%02u-%02u %02u:%02u:%02u", 
				 GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond() );
	}

	return buffer;
}

std::string CASDateTime::StrDate() const
{
	char buffer[ 32 ];

	sprintf( buffer, "%04u-%02u-%02u", GetYear(), GetMonth(), GetDay() );

	return buffer;
}

int CASDateTime::Compare( const CASDateTime& time ) const
{
	if( GetYear() < time.GetYear() )
		return -1;

	if( GetYear() > time.GetYear() )
		return 1;

	if( GetMonth() < time.GetMonth() )
		return -1;

	if( GetMonth() > time.GetMonth() )
		return 1;

	if( GetDay() < time.GetDay() )
		return -1;

	if( GetDay() > time.GetDay() )
		return 1;

	return CASTime::Compare( time );
}

bool CASDateTime::operator<( const CASDateTime& time ) const
{
	return Compare( time ) < 0;
}

bool CASDateTime::operator<=( const CASDateTime& time ) const
{
	return Compare( time ) <= 0;
}

bool CASDateTime::operator>( const CASDateTime& time ) const
{
	return Compare( time ) > 0;
}

bool CASDateTime::operator>=( const CASDateTime& time ) const
{
	return Compare( time ) >= 0;
}

bool CASDateTime::operator==( const CASDateTime& time ) const
{
	return Compare( time ) == 0;
}

bool CASDateTime::operator!=( const CASDateTime& time ) const
{
	return Compare( time ) != 0;
}

CASDateTime CASDateTime::Now()
{
	const time_t time = ::time( nullptr );
	return CASDateTime( *localtime( &time ) );
}

CASDateTime CASDateTime::NowUTC()
{
	const time_t time = ::time( nullptr );
	return CASDateTime( *gmtime( &time ) );
}

bool CASDateTime::IsLeapYear( uint16_t uiYear )
{
	return 
		!( uiYear % 400 ) || 
		( !( uiYear % 4 ) && ( uiYear % 100 ) );
}

uint16_t CASDateTime::GetDaysInYear( uint16_t uiYear )
{
	return IsLeapYear( uiYear ) ? 366 : 365;
}

uint8_t CASDateTime::GetDaysInMonth( uint16_t uiYear, uint8_t uiMonth )
{
	uint8_t uiDays = g_uiMonthLengths[ uiMonth - 1 ];

	if( uiMonth == 2 && IsLeapYear( uiYear ) )
		++uiDays;

	return uiDays;
}

bool CASDateTime::IsValidDate( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay )
{
	if( uiYear == 0 || uiMonth == 0 || uiDay == 0 )
		return false;

	if( uiMonth > 12 )
		return false;

	return uiDay <= GetDaysInMonth( uiYear, uiMonth );
}

uint16_t CASDateTime::GetDayOfYear( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay )
{
	uint16_t uiDayOfYear = 0;

	for( uint8_t uiCurMonth = 1; uiCurMonth < uiMonth; ++uiCurMonth )
	{
		uiDayOfYear += GetDaysInMonth( uiYear, uiCurMonth );
	}

	uiDayOfYear += uiDay;

	return uiDayOfYear;
}

CASDateTime CASDateTime::GetReverseDayOfYear( uint16_t uiYear, uint16_t uiDayOfYear )
{
	uint8_t uiMonth;

	for( uiMonth = 1; uiMonth < 12; ++uiMonth )
	{
		uint16_t uiDays = GetDaysInMonth( uiYear, uiMonth );

		if( uiDays >= uiDayOfYear )
			break;

		uiDayOfYear -= uiDays;
	}

	return CASDateTime( uiYear, uiMonth, static_cast<uint8_t>( uiDayOfYear ) );
}