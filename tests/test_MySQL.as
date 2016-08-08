
class Database
{
	private MySQLConnection@ m_pConnection = null;
	
	void Connect()
	{
		@m_pConnection = SQL.CreateMySQLConnection( "localhost", "root", "", "TestDB" );
		
		if( m_pConnection !is null )
		{
			bool bSuccess = m_pConnection.Query(
				"CREATE DATABASE IF NOT EXISTS TestDB;",
				MySQLQueryCallback( this.CreatedDB )
			);
			
			Print( "Created database query: %1\n", bSuccess ? "yes" : "no" );
		}
	}
	
	private void CreatedDB( MySQLQuery@ pQuery )
	{
		//This has no effect due to connections being per-query.
		bool bSuccess = m_pConnection.Query(
			"USE TestDB;",
			MySQLQueryCallback( this.SelectedDB )
		);
		
		Print( "Selected database query: %1\n", bSuccess ? "yes" : "no" );
	}
	
	private void SelectedDB( MySQLQuery@ pQuery )
	{
		bool bSuccess = m_pConnection.Query( 
			"CREATE TABLE IF NOT EXISTS Test("
			"ID INT PRIMARY KEY NOT NULL,"
			"VALUE INT NOT NULL,"
			"STRING TEXT NOT NULL,"
			"date DATE NOT NULL,"
			"time TIME NOT NULL,"
			"datetime DATETIME NOT NULL"
			")",
			MySQLQueryCallback( this.CreatedTable )
		);
		
		Print( "Created query: %1\n", bSuccess ? "yes" : "no" );
	}
	
	private void CreatedTable( MySQLQuery@ pQuery )
	{
		MySQLPreparedStatement@ pStmt = m_pConnection.CreatePreparedStatement( 
		"INSERT INTO Test (ID, VALUE, STRING, date, time, datetime) "
		"VALUES(?, ?, ?, ?, ?, ?), (?, ?, ?, ?, ?, ?)" );
		
		Print( "Created statement: %1\n", pStmt !is null ? "yes" : "no" );
		
		if( pStmt !is null )
		{
			for( int iIndex = 0; iIndex < 2; ++iIndex )
			{
				Print( "Index: %1\n", iIndex + 1 );
				pStmt.BindSigned32( ( iIndex * 6 ) + 0, iIndex + 1 );
				pStmt.BindSigned32( ( iIndex * 6 ) + 1, 10 );
				pStmt.BindString( ( iIndex * 6 ) + 2, "Foo" );
				pStmt.BindDate( ( iIndex * 6 ) + 3, DateTime::Now().GetDate() );
				pStmt.BindTime( ( iIndex * 6 ) + 4, Time::Now() );
				pStmt.BindDateTime( ( iIndex * 6 ) + 5, DateTime::Now() );
			}
			
			pStmt.ExecuteStatement( MySQLResultSetCallback( this.NullResultSetCallback ), MySQLPreparedStatementCallback( this.InsertedValues ) );
		}
	}
	
	private void NullResultSetCallback( MySQLResultSet@ pSet )
	{
		Print( "null result set callback\n" );
	}
	
	private void InsertedValues( MySQLPreparedStatement@ pStmt )
	{
		Print( "Inserted values\n" );
		
		MySQLPreparedStatement@ pStmt2 = m_pConnection.CreatePreparedStatement( "SELECT * FROM Test" );
		
		if( pStmt2 !is null )
		{
			pStmt2.ExecuteStatement( MySQLResultSetCallback( this.Stmt2Callback ) );
		}
		
	}
	
	private void Stmt2Callback( MySQLResultSet@ pResultSet )
	{
		Print( "Statement 2 row callback invoked\n" );
		
		while( pResultSet.Next() )
		{
			Print( "Key: %1, Value: %2, String: %3, Date: %4, Time: %5, DateTime: %6, type: %7\n", 
				pResultSet.GetSigned32( 0 ), 
				pResultSet.GetSigned32( 1 ), 
				pResultSet.GetString( 2 ),
				pResultSet.GetDate( 3 ).StrDate(),
				pResultSet.GetTime( 4 ).StrTime(),
				pResultSet.GetDateTime( 5 ).StrTime(),
				MySQLFieldTypeToString( pResultSet.GetColumnType( 5 ) ) );
		}
	}
}

Database g_Database;

void main()
{	
	g_Database.Connect();
}