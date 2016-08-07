#ifndef ANGELSCRIPT_SCRIPTAPI_CASDATETIME_H
#define ANGELSCRIPT_SCRIPTAPI_CASDATETIME_H

#include <cstdint>
#include <ctime>
#include <string>

#include "CASTime.h"

/**
*	Datetime class. Based on Sylvain Rochette Langlois's MariaDB++ date_time class.
*/
class CASDateTime final : public CASTime
{
public:
	/**
	*	Constructor. Sets the date and time.
	*/
	CASDateTime( uint16_t uiYear = 1970, uint8_t uiMonth = 1, uint8_t uiDay = 1, 
				 uint8_t uiHour = 0, uint8_t uiMinute = 0, uint8_t uiSecond = 0, uint16_t uiMillisecond = 0 );

	/**
	*	Constructor. Sets the time from a tm instance.
	*	@param time Time to set.
	*/
	CASDateTime( const tm& time );

	/**
	*	Constructor. Sets the date and time from a time_t instance.
	*	@param time Time to set.
	*	@param bUseLocalTime Whether to parse the time as local or universal time.
	*/
	CASDateTime( const time_t& time, const bool bUseLocalTime = true );

	/**
	*	Constructor. Sets the date and time from a string.
	*	@param szString Date and time to parse.
	*/
	CASDateTime( const std::string& szString );

	/**
	*	Copy constructor.
	*/
	CASDateTime( const CASDateTime& other ) = default;

	/**
	*	Assignment operator.
	*/
	CASDateTime& operator=( const CASDateTime& other ) = default;

	/**
	*	@return The year.
	*/
	uint16_t GetYear() const { return m_uiYear; }

	/**
	*	@return The month.
	*/
	uint8_t GetMonth() const { return m_uiMonth; }

	/**
	*	@return The day.
	*/
	uint8_t GetDay() const { return m_uiDay; }

	/**
	*	@return The day of the year.
	*/
	uint16_t GetDayOfYear() const;

	/**
	*	Sets the year. Will reset the month and day to January 1st if the new date isn't valid.
	*	@param uiYear Year to set.
	*	@return Whether the year is valid.
	*/
	bool SetYear( uint16_t uiYear );

	/**
	*	Sets the month.
	*	@param uiMonth Month to set.
	*	@return Whether the month is valid.
	*/
	bool SetMonth( uint8_t uiMonth );

	/**
	*	Sets the day.
	*	@param uiDay Day to set.
	*	@return Whether the day is valid.
	*/
	bool SetDay( uint8_t uiDay );

	/**
	*	Sets the day of the year.
	*	@param uiDayOfYear Day of the year to set.
	*	@return Whether the day is valid.
	*/
	bool SetDayOfYear( uint16_t uiDayOfYear );

	/**
	*	Sets the date.
	*	@param uiYear Year to set.
	*	@param uiMonth Month to set.
	*	@param uiDay Day to set.
	*	@return Whether the given date is valid.
	*/
	bool Set( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay );

	/**
	*	Sets the date and time.
	*	@param uiYear Year to set.
	*	@param uiMonth Month to set.
	*	@param uiDay Day to set.
	*	@param uiHour Hour to set.
	*	@param uiMinute Minute to set.
	*	@param uiSecond Second to set.
	*	@param uiMillisecond Millisecond to set.
	*	@return Whether the given date and time is valid.
	*/
	bool Set( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay,
				 uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, uint16_t uiMillisecond );

	/**
	*	Sets the date and time based on the given string.
	*	The string may omit the time component.
	*	@param szString String to parse.
	*	@return Whether the string represents a valid date.
	*/
	bool Set( const std::string& szString );

	/**
	*	Adds years. Will reset the month and day to January 1st if the new date isn't valid.
	*	@param iYears Years to add. Can be negative.
	*	@return Whether the new year is valid.
	*/
	bool AddYears( int32_t iYears );

	/**
	*	Adds months.
	*	@param iMonths Months to add. Can be negative.
	*	@return Whether the new month is valid.
	*/
	bool AddMonths( int32_t iMonths );

	/**
	*	Adds days.
	*	@param iDays Days to add. Can be negative.
	*	@return Whether the new day is valid.
	*/
	bool AddDays( int32_t iDays );

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
	double TimeDifference( const CASDateTime& time ) const;

	/**
	*	Gets the date part of this date time.
	*/
	CASDateTime GetDate() const;

	/**
	*	Gets the time part of this date time.
	*/
	CASTime GetTime() const;

	/**
	*	Gets a string representation of this date time.
	*	@param bWithMillisecond Whether to include the millisecond.
	*	@return String representation of this date time.
	*/
	std::string StrTime( const bool bWithMillisecond = false ) const;

	/**
	*	Gets a string representation of this date.
	*	@return String representation of this date.
	*/
	std::string StrDate() const;

	/**
	*	Compares this time and the given time.
	*	@param time Time to compare with.
	*	@return 0 if the times are identical,
	*			a negative value if this time is further in the past than the given time,
	*			a positive value if this time is further in the future than the given time.
	*/
	int Compare( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is further in the past than the given time.
	*/
	bool operator<( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is further in the past or equal to the given time.
	*/
	bool operator<=( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is further in the future than the given time.
	*/
	bool operator>( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is further in the future or equal to the given time.
	*/
	bool operator>=( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is equal to the given time.
	*/
	bool operator==( const CASDateTime& time ) const;

	/**
	*	@return Whether this time is not equal to the given time.
	*/
	bool operator!=( const CASDateTime& time ) const;

	/**
	*	@return The current date and time.
	*/
	static CASDateTime Now();

	/**
	*	@return The current date and time, in universal time.
	*/
	static CASDateTime NowUTC();

	/**
	*	Checks whether the given year is a leap year.
	*/
	static bool IsLeapYear( uint16_t uiYear );

	/**
	*	Gets the number of days in the given year.
	*/
	static uint16_t GetDaysInYear( uint16_t uiYear );

	/**
	*	Gets the number of days in the given month. The year is needed to account for leap days.
	*/
	static uint8_t GetDaysInMonth( uint16_t uiYear, uint8_t uiMonth );

	/**
	*	Checks whether the given date is valid.
	*/
	static bool IsValidDate( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay );

	/**
	*	Gets the day of year for the given date.
	*/
	static uint16_t GetDayOfYear( uint16_t uiYear, uint8_t uiMonth, uint8_t uiDay );

	/**
	*	Gets the date from a year and a day of the year.
	*/
	static CASDateTime GetReverseDayOfYear( uint16_t uiYear, uint16_t uiDayOfYear );

private:
	uint16_t m_uiYear	= 1970;
	uint8_t m_uiMonth	= 1;
	uint8_t m_uiDay		= 1;
};

#endif //ANGELSCRIPT_SCRIPTAPI_CASDATETIME_H