# -*- mode: cmake; -*-
# locates the popt library
# This file defines:
# * POPT_FOUND if popt was found
# * POPT_LIBRARY The lib to link to (currently only a static unix lib) 
# * POPT_INCLUDE_DIR

if (NOT POPT_FIND_QUIETLY)
  message(STATUS "FindPopt check")
endif (NOT POPT_FIND_QUIETLY)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  #  include(FindPackageHelper)
  #  check_package(FhgLog fhglog/fhglog.hpp fhglog 1.0)

  find_path (POPT_INCLUDE_DIR
    NAMES "popt.h"
    HINTS ${POPT_HOME} ENV POPT_HOME
    PATH_SUFFIXES include
  )

  find_library (POPT_LIBRARY
    NAMES libpopt.a
    HINTS ${POPT_HOME} ENV POPT_HOME
    PATH_SUFFIXES lib
  )

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # On MacOS
  find_library (POPT_LIBRARY_SHARED
    NAMES libpopt.dylib
    HINTS ${POPT_HOME} ENV POPT_HOME
    PATH_SUFFIXES lib
    )
  ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # On Linux
  find_library (POPT_LIBRARY_SHARED
    NAMES libpopt.so
    HINTS ${POPT_HOME} ENV POPT_HOME
    PATH_SUFFIXES lib64 lib
    )
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


if (POPT_INCLUDE_DIR AND POPT_LIBRARY_SHARED)
  set (POPT_FOUND TRUE)
  if (NOT POPT_FIND_QUIETLY)
    message (STATUS "Found popt headers in ${POPT_INCLUDE_DIR} and libraries ${POPT_LIBRARY} ${POPT_LIBRARY_SHARED}")
  endif (NOT POPT_FIND_QUIETLY)
else (POPT_INCLUDE_DIR AND POPT_LIBRARY_SHARED)
  if (POPT_FIND_REQUIRED)
    message (FATAL_ERROR "popt could not be found!")
  endif (POPT_FIND_REQUIRED)
endif (POPT_INCLUDE_DIR AND POPT_LIBRARY_SHARED)

else(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(POPT_FOUND true)
  set(POPT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/popt ${CMAKE_BINARY_DIR}/popt)
  set(POPT_LIBRARY_DIR "")
  set(POPT_LIBRARY fhglog)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
