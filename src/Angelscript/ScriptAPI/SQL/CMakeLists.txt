add_sources(
	ASSQL.h
	CASSQLThreadPool.h
	CASSQLThreadPool.cpp
	CASSQLThreadQueue.h
	CASSQLThreadQueue.cpp
	IASSQLASyncCommand.h
	IASSQLASyncItem.h
)

add_subdirectory( MySQL )
add_subdirectory( SQLite )