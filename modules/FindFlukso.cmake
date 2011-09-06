# -*- mode: cmake; -*-
# locates the libflukso
# This file defines:
# * FLUKSO_FOUND if howl was found
# * FLUKSO_LIBRARY The lib to link to (currently only a static unix lib) 
# * FLUKSO_INCLUDE_DIR

if (NOT FLUKSO_FIND_QUIETLY)
	message(STATUS "FindFlukso check")
endif (NOT FLUKSO_FIND_QUIETLY)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
#  include(FindPackageHelper)
#  check_package(FhgLog fhglog/fhglog.hpp fhglog 1.0)

	find_path (FLUKSO_INCLUDE_DIR
			NAMES "flukso/flukso.hpp"
			NAMES "flukso/config.hpp"
			NAMES "flukso/formatter.hpp"
			NAMES "flukso/outputfilter.hpp"
			HINTS ${FLUKSO_HOME} ENV FLUKSO_HOME
			PATH_SUFFIXES include
			)

	find_library (FLUKSO_LIBRARY
			NAMES libflukso.a
			HINTS ${FLUKSO_HOME} ENV FLUKSO_HOME
			PATH_SUFFIXES lib
			)

	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# On MacOS
	find_library (FLUKSO_LIBRARY_SHARED
			NAMES libflukso.dylib
			HINTS ${FLUKSO_HOME} ENV FLUKSO_HOME
			PATH_SUFFIXES lib
			)
	ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# On Linux
	find_library (FLUKSO_LIBRARY_SHARED
			NAMES libflukso.so
			HINTS ${FLUKSO_HOME} ENV FLUKSO_HOME
			PATH_SUFFIXES lib
			)
	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


	if (FLUKSO_INCLUDE_DIR AND FLUKSO_LIBRARY)
	set (FLUKSO_FOUND TRUE)
if (NOT FLUKSO_FIND_QUIETLY)
	message (STATUS "Found libflukso headers in ${FLUKSO_INCLUDE_DIR} and libraries ${FLUKSO_LIBRARY} ${FLUKSO_LIBRARY_SHARED}")
	endif (NOT FLUKSO_FIND_QUIETLY)
	else (FLUKSO_INCLUDE_DIR AND FLUKSO_LIBRARY)
#if (FLUKSO_FIND_REQUIRED)
	message (FATAL_ERROR "libflukso could not be found! Cannot compile without. Try setting $FLUKSO_HOME")
#endif (FLUKSO_FIND_REQUIRED)
endif (FLUKSO_INCLUDE_DIR AND FLUKSO_LIBRARY)

else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	set(FLUKSO_FOUND true)
	set(FLUKSO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libflukso ${CMAKE_BINARY_DIR}/libflukso)
	set(FLUKSO_LIBRARY_DIR "")
	set(FLUKSO_LIBRARY libflukso)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
