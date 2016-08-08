add_sources(
	ASSQL.h
	ASSQL.cpp
	CASSQLThreadPool.h
	CASSQLThreadPool.cpp
	CASSQLThreadQueue.h
	CASSQLThreadQueue.cpp
	IASSQLASyncCommand.h
	IASSQLASyncItem.h
	IASSQLQuery.h
)

add_subdirectory( MySQL )
add_subdirectory( SQLite )