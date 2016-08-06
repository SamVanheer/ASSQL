
SQLiteConnection@ g_pConnection = null;

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
			"VALUE INT NOT NULL,"
			"STRING TEXT NOT NULL,"
			"LARGEVAL INT64 NOT NULL,"
			"OPT TEXT"
			")",
			@QueryCallback
		);
		
		Print( "Created query: %1\n", bSuccess ? "yes" : "no" );
		
		SQLitePreparedStatement@ pStmt = g_pConnection.CreatePreparedStatement( "INSERT INTO Test (ID, VALUE, STRING, LARGEVAL, OPT) VALUES(1, ?, ?, ?, ?)" );
		
		Print( "Created statement: %1\n", pStmt !is null ? "yes" : "no" );
		
		if( pStmt !is null )
		{
			pStmt.BindSigned32( 1, 10 );
			pStmt.BindString( 2, "Hello" );
			pStmt.BindSigned64( 3, 0xFFFFFFFFFF );
			pStmt.BindString( 4, "Optional" );
			
			pStmt.ExecuteStatement( null, @StmtCallback );
		}
		
		SQLitePreparedStatement@ pStmt3 = g_pConnection.CreatePreparedStatement( "INSERT INTO Test (ID, VALUE, STRING, LARGEVAL, OPT) VALUES(1, ?, ?, ?, ?)" );
		
		Print( "Created statement: %1\n", pStmt3 !is null ? "yes" : "no" );
		
		if( pStmt3 !is null )
		{
			pStmt3.BindSigned32( 1, 10 );
			pStmt3.BindString( 2, "Hello" );
			pStmt3.BindSigned64( 3, 0xFFFFFFFFFF );
			pStmt3.BindNull( 4 );
			
			pStmt3.ExecuteStatement( null, @StmtCallback );
		}
		
		SQLitePreparedStatement@ pStmt2 = g_pConnection.CreatePreparedStatement( "SELECT * FROM Test" );
		
		if( pStmt2 !is null )
		{
			pStmt2.ExecuteStatement( @RowCallback );
		}
	}
}

void QueryCallback( SQLQuery@ pQuery )
{
	Print( "Query callback invoked\n" );
}

void StmtCallback( SQLitePreparedStatement@ pStmt )
{
	Print( "Statement callback invoked\n" );
}

void RowCallback( SQLiteRow@ pRow )
{
	Print( "Statement 2 row callback invoked, Row %1, ID %2, value %3, text %4, 64 bit integer %5, Optional %6\n", 
		pRow.GetRowIndex(), 
		pRow.GetColumnInt( 0 ), 
		pRow.GetColumnInt( 1 ), 
		pRow.GetColumnText( 2 ),
		pRow.GetColumnInt64( 3 ),
		pRow.GetColumnType( 4 ) != SQLITE_NULL ? pRow.GetColumnText( 4 ) : "NULL" );
	
	for( int iColumn = 0; iColumn < pRow.GetColumnCount(); ++iColumn )
	{
		Print( "Column %1: Type %2\n", iColumn, SQLiteDataTypeToString( pRow.GetColumnType( iColumn ) ) );
	}
}