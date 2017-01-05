###################################################
#                                                 #
#                                                 #
#	Angelscript SQL CMake build file          	  #
#                                                 #
#                                                 #
###################################################
cmake_minimum_required( VERSION 3.6 )

# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
  get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY SRCS_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of source files to be compiled in one library")
  endif()
  # make absolute paths
  set(SRCS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)

# preprocess sources
function( preprocess_sources )
	set(PREP_SRCS PARENT_SCOPE)
	get_property(SRCS GLOBAL PROPERTY SRCS_LIST)
	foreach(s IN LISTS SRCS)
	  file(RELATIVE_PATH rs "${CMAKE_CURRENT_SOURCE_DIR}" "${s}")
	  string(REGEX REPLACE "r$" "" o "${CMAKE_CURRENT_BINARY_DIR}/${rs}")
	  add_custom_command(
		OUTPUT "${o}"
		COMMAND ${CMAKE_COMMAND} -E copy "${s}" "${o}"
		DEPENDS "${s}"
		COMMENT "Creating ${o}"
		VERBATIM
		)
	  list(APPEND PREP_SRCS "${s}")
	endforeach()
	set(PREP_SRCS ${PREP_SRCS} PARENT_SCOPE)
endfunction( preprocess_sources )

function( create_source_groups _src_root_path )
	get_property(SRCS GLOBAL PROPERTY SRCS_LIST)
	foreach(_source IN ITEMS ${SRCS})
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
endfunction( create_source_groups )

#Function to clear the sources list.
function( clear_sources )
	set_property( GLOBAL PROPERTY SRCS_LIST "" )
	set( PREP_SRCS PARENT_SCOPE )
endfunction( clear_sources )

# Function to add include files.
function( add_includes )
  get_property(is_defined GLOBAL PROPERTY INCLUDES_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY INCLUDES_LIST
      BRIEF_DOCS "List of include files"
      FULL_DOCS "List of include files to be compiled in one library")
  endif()
  # make absolute paths
  set(INCLUDES)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND INCLUDES "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY INCLUDES_LIST "${INCLUDES}")
endfunction( add_includes )

# Function to install includes.
function( install_includes _include_root_path )
	get_property(INCLUDES GLOBAL PROPERTY INCLUDES_LIST)
	
	foreach(_include IN ITEMS ${INCLUDES})
		get_filename_component(_include_path "${_include}" PATH)
		file(RELATIVE_PATH _include_path_rel "${_include_root_path}" "${_include_path}")
		string(REPLACE "/" "\\" _group_path "${_include_path_rel}")
		install( FILES "${_include}" DESTINATION "include/${_group_path}" )
	endforeach()
	
	set_property( GLOBAL PROPERTY INCLUDES_LIST "" )
endfunction( install_includes )

macro(configure_msvc_runtime)
  if(MSVC)
    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()

project( AngelscriptSQL )

#Needs static CRT to avoid alloc issues.
set( MSVC_RUNTIME "static" )

configure_msvc_runtime()

set(variables
  CMAKE_C_FLAGS_DEBUG
  CMAKE_C_FLAGS_MINSIZEREL
  CMAKE_C_FLAGS_RELEASE
  CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG
  CMAKE_CXX_FLAGS_MINSIZEREL
  CMAKE_CXX_FLAGS_RELEASE
  CMAKE_CXX_FLAGS_RELWITHDEBINFO
)
message(STATUS "Initial build flags:")
foreach(variable ${variables})
  message(STATUS "  '${variable}': ${${variable}}")
endforeach()
message(STATUS "")

if( UNIX )
	set( LINUX_32BIT_FLAG "-m32" )
else()
	set( LINUX_32BIT_FLAG "" )
endif()

#Debug builds should have a distinct name.
if( NOT CMAKE_DEBUG_POSTFIX )
  set( CMAKE_DEBUG_POSTFIX d )
endif()

#C++14 support
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y" )

set( AS_STRING_OBJNAME "string" CACHE STRING "The name of the object type used to represent strings" )

#Find shared dependencies
find_library( ANGELSCRIPTLIB Angelscript PATHS external/Angelscript/lib/ )
find_library( ANGELSCRIPTUTILS AngelscriptUtils PATHS external/Angelscript/lib/ )

find_library( ANGELSCRIPTLIBD Angelscriptd PATHS external/Angelscript/lib/ )
find_library( ANGELSCRIPTUTILSD AngelscriptUtilsd PATHS external/Angelscript/lib/ )

if( NOT WIN32 )

	#This mucking about is needed because CMake can't figure out how to handle libraries not named lib<name>.a
	set( ANGELSCRIPTLIB "-l:\"${ANGELSCRIPTLIB}\"" )
	set( ANGELSCRIPTUTILS "-l:\"${ANGELSCRIPTUTILS}\"" )
	set( ANGELSCRIPTLIBD "-l:\"${ANGELSCRIPTLIBD}\"" )
	set( ANGELSCRIPTUTILSD "-l:\"${ANGELSCRIPTUTILSD}\"" )

endif()

if( NOT WIN32 )
#
#Begin SQLite library
#

MESSAGE( STATUS "Adding SQLite3 library" )

add_subdirectory( external/SQLite )

preprocess_sources()

include_directories(
	external/SQLite/include
)

add_library( sqlite3 SHARED ${PREP_SRCS} )

set_target_properties( sqlite3 PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" LINK_FLAGS "${SQL_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

install( TARGETS sqlite3 DESTINATION lib )
install_includes( "${CMAKE_SOURCE_DIR}/src" )

clear_sources()

#
#End SQLite library
#
endif()

#
#Begin SQL library
#

add_subdirectory( src/Angelscript )

preprocess_sources()

include_directories(
	src
	external/Angelscript/include
	external/SQLite/include
	external/MariaDB/include
	external/CTPL/include
)

add_library( AngelscriptSQL STATIC ${PREP_SRCS} )

target_compile_definitions( AngelscriptSQL PRIVATE
	AS_STRING_OBJNAME="${AS_STRING_OBJNAME}"
)

target_compile_definitions( AngelscriptSQL PRIVATE
	_CRT_SECURE_NO_WARNINGS
)

if( WIN32 )
	find_library( SQLITE3 "sqlite3${CMAKE_STATIC_LIBRARY_SUFFIX}" PATHS external/SQLite/lib )
	find_library( MARIADB "mariadbclient${CMAKE_STATIC_LIBRARY_SUFFIX}" PATHS external/MariaDB/lib )
	set( PTHREAD "" )
else()
	set( SQLITE3 "sqlite3" )
	find_library( MARIADB "${CMAKE_SHARED_LIBRARY_PREFIX}mariadb.so.2" PATHS external/MariaDB/lib )
	set( PTHREAD "pthread" )
endif()

#Link with SQL dependencies
target_link_libraries( AngelscriptSQL 
	optimized "${ANGELSCRIPTLIB}"
	optimized "${ANGELSCRIPTUTILS}"
	debug "${ANGELSCRIPTLIBD}"
	debug "${ANGELSCRIPTUTILSD}"
	"${SQLITE3}"
	"${MARIADB}"
	"${PTHREAD}"
)

set_target_properties( AngelscriptSQL PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" LINK_FLAGS "${SQL_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#Properly named pdb. By default it always uses the base name.
set_target_properties( AngelscriptSQL PROPERTIES COMPILE_PDB_NAME_DEBUG "AngelscriptSQLd" )
#Match the debug version so its relative position is the same.
set_target_properties( AngelscriptSQL PROPERTIES COMPILE_PDB_NAME_RELWITHDEBINFO "AngelscriptSQL" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

install( TARGETS AngelscriptSQL DESTINATION lib )
install_includes( "${CMAKE_SOURCE_DIR}/src" )

#Install the pdb files.
if( WIN32 )
	install( FILES ${CMAKE_BINARY_DIR}/Debug/AngelscriptSQLd.pdb DESTINATION lib CONFIGURATIONS Debug )
	install( FILES ${CMAKE_BINARY_DIR}/RelWithDebInfo/AngelscriptSQL.pdb DESTINATION lib CONFIGURATIONS RelWithDebInfo )
endif()

clear_sources()

#
#End SQL library
#

#
#Test exe
#

add_subdirectory( src/test )

preprocess_sources()

add_executable( AngelscriptSQLTest ${PREP_SRCS} )

target_compile_definitions( AngelscriptSQLTest PRIVATE
	_CRT_SECURE_NO_WARNINGS
	AS_STRING_OBJNAME="${AS_STRING_OBJNAME}"
)

target_link_libraries( AngelscriptSQLTest
	AngelscriptSQL
)

set_target_properties( AngelscriptSQLTest PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" LINK_FLAGS "${TEST_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

#Set the startup project to the client
#TODO: does this even work?
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AngelscriptSQLTest )

clear_sources()

#Copy the sqlite3 dll so the exe can get to it
add_custom_command( TARGET AngelscriptSQLTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_SOURCE_DIR}/external/SQLite/lib/sqlite3.dll"
	$<TARGET_FILE_DIR:AngelscriptSQLTest>
)

#Copy the mariadb dll so the exe can get to it
add_custom_command( TARGET AngelscriptSQLTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_SOURCE_DIR}/external/MariaDB/lib/libmariadb.dll"
	$<TARGET_FILE_DIR:AngelscriptSQLTest>
)

#Copy the mariadb library so the exe can get to it
add_custom_command( TARGET AngelscriptSQLTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_CURRENT_SOURCE_DIR}/external/MariaDB/lib/libmariadb.so.2"
	$<TARGET_FILE_DIR:AngelscriptSQLTest>
)

#
#End test exe
#