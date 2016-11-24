add_sources(
	ASSQL.h
	ASSQL.cpp
	CASSQLThreadPool.h
	CASSQLThreadPool.cpp
	CASSQLThreadQueue.h
	CASSQLThreadQueue.cpp
	IASSQLASyncCommand.h
	IASSQLASyncItem.h
)

add_includes(
	ASSQL.h
	CASSQLThreadPool.h
	CASSQLThreadQueue.h
	IASSQLASyncCommand.h
	IASSQLASyncItem.h
)

add_subdirectory( MySQL )
add_subdirectory( SQLite )