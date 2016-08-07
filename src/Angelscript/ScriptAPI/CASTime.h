#ifndef ANGELSCRIPT_SCRIPTAPI_CASTIME_H
#define ANGELSCRIPT_SCRIPTAPI_CASTIME_H

#include <cstdint>
#include <ctime>
#include <string>

/**
*	Time class. Based on Sylvain Rochette Langlois's MariaDB++ time class.
*/
class CASTime
{
public:
	/**
	*	Constructor. Sets the time.
	*	@param uiHour Hour to set.
	*	@param uiMinute Minute to set.
	*	@param uiSecond Second to set.
	*	@param uiMillisecond Millisecond to set.
	*/
	CASTime( uint8_t uiHour = 0, uint8_t uiMinute = 0, uint8_t uiSecond = 0, uint16_t uiMillisecond = 0 );

	/**
	*	Constructor. Sets the time from a tm instance.
	*	@param time Time to set.
	*/
	CASTime( const tm& time );

	/**
	*	Constructor. Sets the time from a time_t instance.
	*	@param time Time to set.
	*	@param bUseLocalTime Whether to parse the time as local or universal time.
	*/
	CASTime( const time_t& time, const bool bUseLocalTime = true );

	/**
	*	Constructor. Sets the time from a string.
	*	@param szString Time to parse.
	*/
	CASTime( const std::string& szString );

	/**
	*	Copy constructor.
	*/
	CASTime( const CASTime& other ) = default;

	/**
	*	Assignment operator.
	*/
	CASTime& operator=( const CASTime& other ) = default;

	/**
	*	@return The hour.
	*/
	uint8_t GetHour() const { return m_uiHour; }

	/**
	*	@return The minute.
	*/
	uint8_t GetMinute() const { return m_uiMinute; }

	/**
	*	@return The second.
	*/
	uint8_t GetSecond() const { return m_uiSecond; }

	/**
	*	@return The millisecond.
	*/
	uint16_t GetMillisecond() const { return m_uiMillisecond; }

	/**
	*	Sets the hour.
	*	@param uiHour Hour to set.
	*	@return Whether the hour is valid.
	*/
	bool SetHour( uint8_t uiHour );

	/**
	*	Sets the minute.
	*	@param uiMinute Minute to set.
	*	@return Whether the minute is valid.
	*/
	bool SetMinute( uint8_t uiMinute );

	/**
	*	Sets the second.
	*	@param uiSecond Second to set.
	*	@return Whether the second is valid.
	*/
	bool SetSecond( uint8_t uiSecond );

	/**
	*	Sets the millisecond.
	*	@param uiMillisecond Millisecond to set.
	*	@return Whether the millisecond is valid.
	*/
	bool SetMillisecond( uint16_t uiMillisecond );

	/**
	*	Sets the time.
	*	@param uiHour Hour to set.
	*	@param uiMinute Minute to set.
	*	@param uiSecond Second to set.
	*	@param uiMillisecond Millisecond to set.
	*	@return Whether the given time is valid.
	*/
	bool Set( uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMilliseconds );

	/**
	*	Sets the time from a string.
	*	@param szString String to parse.
	*	@return Whether the given string represents a valid time.
	*/
	bool Set( const std::string& szString );

	/**
	*	Adds hours.
	*	@param iHours Hours to add. Can be negative.
	*	@return Whether the new hour is valid.
	*/
	bool AddHours( int32_t iHours );

	/**
	*	Adds minutes.
	*	@param iMinutes Minutes to add. Can be negative.
	*	@return Whether the new minute is valid.
	*/
	bool AddMinutes( int32_t iMinutes );

	/**
	*	Adds seconds.
	*	@param iSeconds Seconds to add. Can be negative.
	*	@return Whether the new second is valid.
	*/
	bool AddSeconds( int32_t iSeconds );

	/**
	*	Adds milliseconds.
	*	@param iMilliseconds Milliseconds to add. Can be negative.
	*	@return Whether the new milliseconds is valid.
	*/
	bool AddMilliseconds( int32_t iMilliseconds );

	/**
	*	@return Time as a tm instance. This will be a time on the day that the Unix epoch started.
	*/
	tm Totm() const;

	/**
	*	Converts this time to a time_t instance.
	*/
	time_t MkTime() const;

	/**
	*	Gets the difference between this time and the given time.
	*	@param time Time to return the difference between.
	*/
	double TimeDifference( const CASTime& time ) const;

	/**
	*	Gets a string representation of this time.
	*	@param bWithMillisecond Whether to include the millisecond.
	*	@return String representation of this time.
	*/
	std::string StrTime( const bool bWithMillisecond = false ) const;

	/**
	*	Compares this time and the given time.
	*	@param time Time to compare with.
	*	@return 0 if the times are identical, 
	*			a negative value if this time is further in the past than the given time, 
	*			a positive value if this time is further in the future than the given time.
	*/
	int Compare( const CASTime& time ) const;

	/**
	*	@return Whether this time is further in the past than the given time.
	*/
	bool operator<( const CASTime& time ) const;

	/**
	*	@return Whether this time is further in the past or equal to the given time.
	*/
	bool operator<=( const CASTime& time ) const;

	/**
	*	@return Whether this time is further in the future than the given time.
	*/
	bool operator>( const CASTime& time ) const;

	/**
	*	@return Whether this time is further in the future or equal to the given time.
	*/
	bool operator>=( const CASTime& time ) const;

	/**
	*	@return Whether this time is equal to the given time.
	*/
	bool operator==( const CASTime& time ) const;

	/**
	*	@return Whether this time is not equal to the given time.
	*/
	bool operator!=( const CASTime& time ) const;

	/**
	*	@return The current time.
	*/
	static CASTime Now();

	/**
	*	@return The current time, in universal time.
	*/
	static CASTime NowUTC();

private:
	uint8_t m_uiHour			= 0;
	uint8_t m_uiMinute			= 0;
	uint8_t m_uiSecond			= 0;
	uint16_t m_uiMillisecond	= 0;
};

#endif //ANGELSCRIPT_SCRIPTAPI_CASTIME_H