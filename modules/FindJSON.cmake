# -*- mode: cmake; -*-
# locates the json library
# This file defines:
# * JSON_FOUND if json was found
# * JSON_LIBRARY The lib to link to (currently only a static unix lib) 
# * JSON_INCLUDE_DIR

if (NOT JSON_FIND_QUIETLY)
  message(STATUS "FindJSON check")
endif (NOT JSON_FIND_QUIETLY)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  #  include(FindPackageHelper)
  #  check_package(FhgLog fhglog/fhglog.hpp fhglog 1.0)

  find_path (JSON_INCLUDE_DIR
    NAMES "json/json.h"
    HINTS ${JSON_HOME} ENV JSON_HOME
    PATH_SUFFIXES include
  )

  find_library (JSON_LIBRARY
    NAMES libjson.a
    HINTS ${JSON_HOME} ENV JSON_HOME
    PATH_SUFFIXES lib
  )

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # On MacOS
    find_library (JSON_LIBRARY_SHARED
      NAMES libjson.dylib
      HINTS ${JSON_HOME} ENV JSON_HOME
      PATH_SUFFIXES lib
      )
  ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # On Linux
    find_library (JSON_LIBRARY_SHARED
      NAMES libjson.dylib
      HINTS ${JSON_HOME} ENV JSON_HOME
      PATH_SUFFIXES lib
      )
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


if (JSON_INCLUDE_DIR AND JSON_LIBRARY)
  set (JSON_FOUND TRUE)
  if (NOT JSON_FIND_QUIETLY)
    message (STATUS "Found json headers in ${JSON_INCLUDE_DIR} and libraries ${JSON_LIBRARY} ${JSON_LIBRARY_SHARED}")
  endif (NOT JSON_FIND_QUIETLY)
else (JSON_INCLUDE_DIR AND JSON_LIBRARY)
  #if (JSON_FIND_REQUIRED)
    message (FATAL_ERROR "json could not be found! Cannot compile without. Try setting $JSON_HOME")
    #endif (JSON_FIND_REQUIRED)
endif (JSON_INCLUDE_DIR AND JSON_LIBRARY)

else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(JSON_FOUND true)
  set(JSON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/json ${CMAKE_BINARY_DIR}/json)
  set(JSON_LIBRARY_DIR "")
  set(JSON_LIBRARY json)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
