# -*- mode: cmake; -*-
# locates the howl library
# This file defines:
# * HOWL_FOUND if howl was found
# * HOWL_LIBRARY The lib to link to (currently only a static unix lib) 
# * HOWL_INCLUDE_DIR

if (NOT HOWL_FIND_QUIETLY)
	message(STATUS "FindHOWL check")
endif (NOT HOWL_FIND_QUIETLY)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
#  include(FindPackageHelper)
#  check_package(FhgLog fhglog/fhglog.hpp fhglog 1.0)

	find_path (HOWL_INCLUDE_DIR
			NAMES "howl/howl.h"
			HINTS ${HOWL_HOME} ENV HOWL_HOME
			PATH_SUFFIXES include
			)

	find_library (HOWL_LIBRARY
			NAMES libhowl.a
			HINTS ${HOWL_HOME} ENV HOWL_HOME
			PATH_SUFFIXES lib
			)

	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# On MacOS
	find_library (HOWL_LIBRARY_SHARED
			NAMES libholw.dylib
			HINTS ${HOWL_HOME} ENV HOWL_HOME
			PATH_SUFFIXES lib
			)
	ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# On Linux
	find_library (HOWL_LIBRARY_SHARED
			#NAMES libhowl.dylib
			NAMES libhowl.so
			HINTS ${HOWL_HOME} ENV HOWL_HOME
			PATH_SUFFIXES lib
			)
	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


	if (HOWL_INCLUDE_DIR AND HOWL_LIBRARY)
	set (HOWL_FOUND TRUE)
if (NOT HOWL_FIND_QUIETLY)
	message (STATUS "Found howl headers in ${HOWL_INCLUDE_DIR} and libraries ${HOWL_LIBRARY} ${HOWL_LIBRARY_SHARED}")
	endif (NOT HOWL_FIND_QUIETLY)
	else (HOWL_INCLUDE_DIR AND HOWL_LIBRARY)
#if (HOWL_FIND_REQUIRED)
	message (FATAL_ERROR "howl could not be found! Cannot compile without. Try setting $HOWL_HOME")
#endif (HOWL_FIND_REQUIRED)
endif (HOWL_INCLUDE_DIR AND HOWL_LIBRARY)

else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	set(HOWL_FOUND true)
	set(HOWL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/howl ${CMAKE_BINARY_DIR}/howl)
	set(HOWL_LIBRARY_DIR "")
	set(HOWL_LIBRARY howl)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
