
SQLConnection@ g_pConnection = null;

void main()
{
	SQLConnection@ pConnection = SQL.CreateSQLiteConnection( "test_SQLite.sqlite" );
	
	if( pConnection !is null )
	{
		Print( "Connection created\n" );
		
		if( pConnection.IsOpen() )
		{
			Print( "Connection is open\n" );
			pConnection.Close();
			
			if( !pConnection.IsOpen() )
			{
				Print( "Connection closed\n" );
			}
			else
			{
				Print( "Connection was not closed\n" );
			}
		}
		else
		{
			Print( "Connection was closed\n" );
		}
		
		@pConnection = null;
	}
	else
	{
		Print( "Connection was null!\n" );
	}
	
	@g_pConnection = SQL.CreateSQLiteConnection( "test_SQLite.sqlite" );
	
	if( g_pConnection !is null && g_pConnection.IsOpen() )
	{
		const bool bSuccess = g_pConnection.Query( 
			"CREATE TABLE IF NOT EXISTS Test("
			"ID INT PRIMARY_KEY NOT NULL,"
			"VALUE INT NOT NULL"
			")",
			@QueryCallback
		);
		
		Print( "Created query: %1\n", bSuccess ? "yes" : "no" );
		
		SQLPreparedStatement@ pStmt = g_pConnection.CreatePreparedStatement( "INSERT INTO Test (ID, VALUE) VALUES(1, ?)" );
		
		Print( "Created statement: %1\n", pStmt !is null ? "yes" : "no" );
		
		if( pStmt !is null )
		{
			pStmt.Bind( 1, 10 );
			
			pStmt.ExecuteStatement( @StmtCallback );
		}
	}
}

void QueryCallback( SQLQuery@ pQuery )
{
	Print( "Query callback invoked\n" );
}

void StmtCallback( SQLPreparedStatement@ pStmt )
{
	Print( "Statement callback invoked\n" );
}