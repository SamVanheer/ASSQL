
class Database
{
	private MySQLConnection@ m_pConnection = null;
	
	void Connect()
	{
		@m_pConnection = SQL.CreateMySQLConnection( "localhost", "root", "" );
		
		if( m_pConnection !is null && m_pConnection.IsOpen() )
		{
			bool bSuccess = m_pConnection.Query(
				"CREATE DATABASE IF NOT EXISTS TestDB;",
				SQLQueryCallback( this.CreatedDB )
			);
			
			Print( "Created database query: %1\n", bSuccess ? "yes" : "no" );
		}
	}
	
	private void CreatedDB( SQLQuery@ pQuery )
	{
		bool bSuccess = m_pConnection.Query(
			"USE TestDB;",
			SQLQueryCallback( this.SelectedDB )
		);
		
		Print( "Selected database query: %1\n", bSuccess ? "yes" : "no" );
	}
	
	private void SelectedDB( SQLQuery@ pQuery )
	{
		bool bSuccess = m_pConnection.Query( 
			"CREATE TABLE IF NOT EXISTS Test("
			"ID INT PRIMARY KEY NOT NULL,"
			"VALUE INT NOT NULL"
			")",
			SQLQueryCallback( this.CreatedTable )
		);
		
		Print( "Created query: %1\n", bSuccess ? "yes" : "no" );
	}
	
	private void CreatedTable( SQLQuery@ pQuery )
	{
		MySQLPreparedStatement@ pStmt = m_pConnection.CreatePreparedStatement( "INSERT INTO Test (ID, VALUE) VALUES(1, ?)" );
		
		Print( "Created statement: %1\n", pStmt !is null ? "yes" : "no" );
		
		if( pStmt !is null )
		{
			pStmt.Bind( 0, 10 );
			
			pStmt.ExecuteStatement( MySQLPreparedStatementCallback( this.InsertedValues ) );
		}
	}
	
	private void InsertedValues( MySQLPreparedStatement@ pStmt )
	{
		MySQLPreparedStatement@ pStmt2 = m_pConnection.CreatePreparedStatement( "SELECT * FROM Test" );
		
		if( pStmt2 !is null )
		{
			pStmt2.ExecuteStatement( MySQLPreparedStatementCallback( this.Stmt2Callback ) );
		}
	}
	
	private void Stmt2Callback( MySQLPreparedStatement@ pStmt )
	{
		Print( "Statement 2 row callback invoked\n" );
	}
}

Database g_Database;

void main()
{
	SQLConnection@ pConnection = SQL.CreateMySQLConnection( "localhost", "root", "" );
	
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
	
	g_Database.Connect();
}