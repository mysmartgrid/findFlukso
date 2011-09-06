# -*- mode: cmake; -*-
# locates the curl library
# This file defines:
# * CURL_FOUND if curl was found
# * CURL_LIBRARY The lib to link to (currently only a static unix lib) 
# * CURL_INCLUDE_DIR

if (NOT CURL_FIND_QUIETLY)
  message(STATUS "FindCURL check")
endif (NOT CURL_FIND_QUIETLY)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  #  include(FindPackageHelper)
  #  check_package(FhgLog fhglog/fhglog.hpp fhglog 1.0)

  find_path (CURL_INCLUDE_DIR
    NAMES "curl/curl.h"
    HINTS ${CURL_HOME} ENV CURL_HOME
    PATH_SUFFIXES include
  )

  find_library (CURL_LIBRARY
    NAMES libcurl.a
    HINTS ${CURL_HOME} ENV CURL_HOME
    PATH_SUFFIXES lib
  )

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # On MacOS
    find_library (CURL_LIBRARY_SHARED
      NAMES libcurl.dylib
      HINTS ${CURL_HOME} ENV CURL_HOME
      PATH_SUFFIXES lib
      )
  ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # On Linux
    find_library (CURL_LIBRARY_SHARED
      NAMES libcurl.so
      HINTS ${CURL_HOME} ENV CURL_HOME
      PATH_SUFFIXES lib
      )
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


if (CURL_INCLUDE_DIR AND CURL_LIBRARY)
  set (CURL_FOUND TRUE)
  if (NOT CURL_FIND_QUIETLY)
    message (STATUS "Found curl headers in ${CURL_INCLUDE_DIR} and libraries ${CURL_LIBRARY} ${CURL_LIBRARY_SHARED}")
  endif (NOT CURL_FIND_QUIETLY)
else (CURL_INCLUDE_DIR AND CURL_LIBRARY)
  if (CURL_FIND_REQUIRED)
    message (FATAL_ERROR "curl could not be found!")
  endif (CURL_FIND_REQUIRED)
endif (CURL_INCLUDE_DIR AND CURL_LIBRARY)

else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(CURL_FOUND true)
  set(CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/curl ${CMAKE_BINARY_DIR}/curl)
  set(CURL_LIBRARY_DIR "")
  set(CURL_LIBRARY curl)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
