add_sources(
	ASSQL.h
	ASSQL.cpp
	ASSQLThreading.h
	CASSQLThreadPool.h
	CASSQLThreadPool.cpp
	CASSQLThreadQueue.h
	CASSQLThreadQueue.cpp
	IASSQLASyncItem.h
	IASSQLConnection.h
	IASSQLPreparedStatement.h
	IASSQLQuery.h
	IASSQLRow.h
)

add_subdirectory( SQLite )