#include "CASTime.h"

// JonnyBoy0719: VS2013 fix
#include "Angelscript\util\ASPlatform.h"

CASTime::CASTime( uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	Set( uiHour, uiMinute, uiSecond, uiMillisecond );
}

CASTime::CASTime( const tm& time )
{
	Set( time.tm_hour, time.tm_min, time.tm_sec, 0 );
}

CASTime::CASTime( const time_t& time, const bool bUseLocalTime )
	: CASTime( *( bUseLocalTime ? localtime : gmtime )( &time ) )
{
}

CASTime::CASTime( const std::string& szString )
{
	Set( szString );
}

bool CASTime::SetHour( uint8_t uiHour )
{
	if( uiHour >= 24 )
		return false;

	m_uiHour = uiHour;

	return true;
}

bool CASTime::SetMinute( uint8_t uiMinute )
{
	if( uiMinute >= 60 )
		return false;

	m_uiMinute = uiMinute;

	return true;
}

bool CASTime::SetSecond( uint8_t uiSecond )
{
	if( uiSecond >= 60 )
		return false;

	m_uiSecond = uiSecond;

	return true;
}

bool CASTime::SetMillisecond( uint16_t uiMillisecond )
{
	if( uiMillisecond >= 1000 )
		return false;

	m_uiMillisecond = uiMillisecond;

	return true;
}

bool CASTime::Set( uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond )
{
	if( uiHour >= 24 || uiMinute >= 60 || uiSecond >= 60 || uiMillisecond >= 1000 )
		return false;

	m_uiHour			= uiHour;
	m_uiMinute			= uiMinute;
	m_uiSecond			= uiSecond;
	m_uiMillisecond		= uiMillisecond;

	return true;
}

bool CASTime::Set( const std::string& szString )
{
	if( szString.empty() )
		return false;

	const size_t uiLength = szString.length();

	if( uiLength < 2 )
		return false;

	const uint8_t uiHour = static_cast<uint8_t>( std::stoul( szString.substr( 0, 2 ) ) );

	uint8_t uiMinute = 0;

	double flSecond = 0;

	if( uiLength >= 3 )
	{
		uiMinute = static_cast<uint8_t>( std::stoul( szString.substr( 3, 2 ) ) );

		if( uiLength >= 6 )
			flSecond = std::stod( szString.substr( 6 ) );
	}

	uint16_t uiMillisecond = static_cast<uint16_t>( ( flSecond - static_cast<uint8_t>( flSecond ) ) * 1000 );

	return Set( uiHour, uiMinute, static_cast<uint8_t>( flSecond ), uiMillisecond );
}

bool CASTime::AddHours( int32_t iHours )
{
	if( iHours == 0 )
		return true;

	iHours = ( iHours % 24 ) + GetHour();

	if( iHours >= 24 )
	{
		iHours %= 24;
	}
	else if( iHours < 0 )
	{
		iHours = 24 - iHours;
	}

	return SetHour( iHours );
}

bool CASTime::AddMinutes( int32_t iMinutes )
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

	CASTime temp( *this );

	if( !temp.AddHours( iHours ) )
		return false;

	if( !temp.SetMinute( iMinutes ) )
		return false;

	*this = temp;

	return true;
}

bool CASTime::AddSeconds( int32_t iSeconds )
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

	CASTime temp( *this );

	if( !temp.AddMinutes( iMinutes ) )
		return false;

	if( !temp.SetSecond( iSeconds ) )
		return false;

	*this = temp;

	return true;
}

bool CASTime::AddMilliseconds( int32_t iMilliseconds )
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

	CASTime temp( *this );

	if( !temp.AddSeconds( iSeconds ) )
		return false;

	if( !temp.SetMillisecond( iMilliseconds ) )
		return false;

	*this = temp;

	return true;
}

tm CASTime::Totm() const
{
	tm time;

	time.tm_year = 70;
	time.tm_mon = 0;
	time.tm_mday = 1;
	time.tm_hour = GetHour();
	time.tm_min = GetMinute();
	time.tm_sec = GetSecond();

	return time;
}

time_t CASTime::MkTime() const
{
	tm time = Totm();

	return mktime( &time );
}

double CASTime::TimeDifference( const CASTime& time ) const
{
	return ::difftime( MkTime(), time.MkTime() );
}

std::string CASTime::StrTime( const bool bWithMillisecond ) const
{
	char szBuffer[ 14 ];

	if( bWithMillisecond && GetMillisecond() != 0 )
	{
		snprintf( szBuffer, sizeof( szBuffer ), "%02u:%02u:%02u.%03u", GetHour(), GetMinute(), GetSecond(), GetMillisecond() );
	}
	else
	{
		snprintf( szBuffer, sizeof( szBuffer ), "%02u:%02u:%02u", GetHour(), GetMinute(), GetSecond() );
	}

	return szBuffer;
}

int CASTime::Compare( const CASTime& time ) const
{
	if( GetHour() < time.GetHour() )
		return -1;

	if( GetHour() > time.GetHour() )
		return 1;

	if( GetMinute() < time.GetMinute() )
		return -1;

	if( GetMinute() > time.GetMinute() )
		return 1;

	if( GetSecond() < time.GetSecond() )
		return -1;

	if( GetSecond() > time.GetSecond() )
		return 1;

	if( GetMillisecond() < time.GetMillisecond() )
		return -1;

	if( GetMillisecond() > time.GetMillisecond() )
		return 1;

	return 0;
}

bool CASTime::operator<( const CASTime& time ) const
{
	return Compare( time ) < 0;
}

bool CASTime::operator<=( const CASTime& time ) const
{
	return Compare( time ) <= 0;
}

bool CASTime::operator>( const CASTime& time ) const
{
	return Compare( time ) > 0;
}

bool CASTime::operator>=( const CASTime& time ) const
{
	return Compare( time ) >= 0;
}

bool CASTime::operator==( const CASTime& time ) const
{
	return Compare( time ) == 0;
}

bool CASTime::operator!=( const CASTime& time ) const
{
	return Compare( time ) != 0;
}

CASTime CASTime::Now()
{
	time_t time = ::time( nullptr );

	return CASTime( *::localtime( &time ) );
}

CASTime CASTime::NowUTC()
{
	time_t time = ::time( nullptr );

	return CASTime( *::gmtime( &time ) );
}
