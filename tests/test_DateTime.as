const uint MILLISECOND_INTERVAL = 93;

void main()
{
	TestTime();
	TestDateTime();
}

void TestTime()
{
	Print( "Testing CTime interface\n" );

	CTime now = Time::Now();
	
	Print( "Now: %1\n", now.StrTime( true ) );
	
	Print( "Now UTC: %1\n", Time::NowUTC().StrTime( true ) );
	
	CTime anHourAhead = now;
	anHourAhead.AddHours( 1 );
	
	Print( "An hour from now: " + anHourAhead.StrTime( true ) + '\n' );
	
	CTime temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 1500; uiIndex += MILLISECOND_INTERVAL )
	{
		Print( "Time at +%1 milliseconds: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddMilliseconds( MILLISECOND_INTERVAL );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 60; ++uiIndex )
	{
		Print( "Time at +%1 seconds: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddSeconds( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 60; ++uiIndex )
	{
		Print( "Time at +%1 minutes: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddMinutes( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 24; ++uiIndex )
	{
		Print( "Time at +%1 hours: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddHours( 1 );
	}
	
	const string szString = now.StrTime();
	
	temp = CTime( szString );
	
	Print( "Time: %1\n", temp.StrTime( true ) );
	
	CTime endTime( 10, 5, 17 );
	
	Print( "Time: %1\n", endTime.StrTime( true ) );
	
	Print( "Time: %1:%2:%3:%4\n", endTime.GetHour(), endTime.GetMinute(), endTime.GetSecond(), endTime.GetMillisecond() );
	
	Print( "Time difference between %1 and %2: %3\n", now.StrTime(), anHourAhead.StrTime(), now.TimeDifference( anHourAhead ) );
	
	CTime startTime( 12, 10, 37 );
	Print( "Time difference between %1 and %2: %3\n", startTime.StrTime(), endTime.StrTime(), startTime.TimeDifference( endTime ) );
	
	Print( "Compare %1 and %2: %3\n", startTime.StrTime(), endTime.StrTime(), startTime.opCmp( endTime ) );
	Print( "Is %1 < than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime < endTime );
	Print( "Is %1 <= than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime <= endTime );
	Print( "Is %1 > than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime > endTime );
	Print( "Is %1 >= than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime >= endTime );
	Print( "Does %1 == %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime == endTime );
	Print( "Does %1 == %2? %3\n", startTime.StrTime(), startTime.StrTime(), startTime == startTime );
	Print( "Does %1 != %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime != endTime );
	
	Print( "End CTime interface tests\n\n\n" );
}

void TestDateTime()
{
	Print( "Testing CDateTime interface\n" );
	
	CDateTime now = DateTime::Now();
	
	Print( "Now: %1\n", now.StrTime( true ) );
	
	Print( "Now UTC: %1\n", DateTime::NowUTC().StrTime( true ) );
	
	CDateTime anHourAhead = now;
	anHourAhead.AddHours( 1 );
	
	Print( "An hour from now: " + anHourAhead.StrTime( true ) + '\n' );
	
	CDateTime temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 1500; uiIndex += MILLISECOND_INTERVAL )
	{
		Print( "Time at +%1 milliseconds: %2\n", uiIndex, temp.StrTime( true ) );
		
		const uint16 uiOld = temp.GetMillisecond();
		
		temp.AddMilliseconds( MILLISECOND_INTERVAL );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 60; ++uiIndex )
	{
		Print( "Time at +%1 seconds: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddSeconds( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 60; ++uiIndex )
	{
		Print( "Time at +%1 minutes: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddMinutes( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 24; ++uiIndex )
	{
		Print( "Time at +%1 hours: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddHours( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex <= 35; ++uiIndex )
	{
		Print( "Time at +%1 days: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddDays( 1 );
	}
	
	temp = now;
	
	for( uint uiIndex = 0; uiIndex < 12; ++uiIndex )
	{
		Print( "Time at +%1 months: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddMonths( 1 );
	}
	
	temp = now;
	
	temp.SetYear( 2016 );
	temp.SetMonth( 2 );
	temp.SetDay( 29 );
	
	const uint YEAR_INTERVAL = 17;
	
	for( uint uiIndex = 0; uiIndex < 100; uiIndex += YEAR_INTERVAL )
	{
		Print( "Time at +%1 years: %2\n", uiIndex, temp.StrTime( true ) );
		
		temp.AddYears( YEAR_INTERVAL );
	}
	
	const string szString = now.StrTime();
	
	temp = CDateTime( szString );
	
	Print( "Time: %1\n", temp.StrTime( true ) );
	
	CDateTime endTime( 10, 5, 17 );
	
	Print( "Time: %1\n", endTime.StrTime( true ) );
	
	Print( "Time: %1:%2:%3:%4\n", endTime.GetHour(), endTime.GetMinute(), endTime.GetSecond(), endTime.GetMillisecond() );
	
	Print( "Time difference between %1 and %2: %3\n", now.StrTime(), anHourAhead.StrTime(), now.TimeDifference( anHourAhead ) );
	
	CDateTime startTime( 12, 10, 37 );
	Print( "Time difference between %1 and %2: %3\n", startTime.StrTime(), endTime.StrTime(), startTime.TimeDifference( endTime ) );
	
	Print( "Compare %1 and %2: %3\n", startTime.StrTime(), endTime.StrTime(), startTime.opCmp( endTime ) );
	Print( "Is %1 < than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime < endTime );
	Print( "Is %1 <= than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime <= endTime );
	Print( "Is %1 > than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime > endTime );
	Print( "Is %1 >= than %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime >= endTime );
	Print( "Does %1 == %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime == endTime );
	Print( "Does %1 == %2? %3\n", startTime.StrTime(), startTime.StrTime(), startTime == startTime );
	Print( "Does %1 != %2? %3\n", startTime.StrTime(), endTime.StrTime(), startTime != endTime );
	
	Print( "Day of year: %1 %2\n", CDateTime().StrTime(), CDateTime().GetDayOfYear() );
	Print( "Day of year: %1 %2\n", now.StrTime(), now.GetDayOfYear() );
	
	CDateTime tomorrow = DateTime::GetReverseDayOfYear( now.GetYear(), now.GetDayOfYear() + 1 );
	
	Print( "Tomorrow: %1\n", tomorrow.StrDate() );
	
	Print( "Current date: %1\n", now.GetDate().StrDate() );
	Print( "Current time: %1\n", now.GetTime().StrTime() );
	
	Print( "End CDateTime interface tests\n\n\n" );
}