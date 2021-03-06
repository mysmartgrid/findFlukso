# -*- mode: cmake; -*-
project(findFlukso)
cmake_minimum_required(VERSION "2.6" FATAL_ERROR)
if(POLICY CMP0011)
	cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

# have the full monty in makefiles
set(CMAKE_VERBOSE_MAKEFILE true)

set(V_MAJOR 0)
set(V_MINOR 1)
set(V_PATCH 0)

# add a path where some libraries might be stored
	set(CMAKE_ADDITIONAL_PATH "$ENV{CMAKE_ADDITIONAL_PATH}" CACHE PATH "Path where many locally installed libraries can be found")

###################################################################################################
# No user settings below
###################################################################################################

# Where are the additional libraries installed? Note: provide includes
# path here, subsequent checks will resolve everything else
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_ADDITIONAL_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_ADDITIONAL_PATH}/lib)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-non-virtual-dtor -g -fomit-frame-pointer")

	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "-O0 -g -ggdb -Wall -Werror -W -Wshadow")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-variable")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-parameter")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused-function")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wunused -Woverloaded-virtual")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wno-system-headers")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -Wno-non-virtual-dtor")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -pg -fprofile-generate ")
	set(CMAKE_CXX_FLAGS_EXPERIMENTAL "${CMAKE_CXX_FLAGS_EXPERIMENTAL} -fprofile-arcs -ftest-coverage")
# gprof and gcov support
	set(CMAKE_CXX_FLAGS_PROFILE "-O0 -g -ggdb -W -Wreturn-type -Wno-shadow")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wunused-variable -Wunused-parameter")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wunused-function -Wunused")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Woverloaded-virtual -Wno-system-headers")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -Wno-non-virtual-dtor -pg -fprofile-arcs")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -ftest-coverage")

# Releases are made with the release build. Optimize code.
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-non-virtual-dtor -Werror")

# allways set these flags
	set(CMAKE_CXX_FLAGS "-Wall ")

# Check for some compiler stuff
include(CMakeBackwardCompatibilityCXX)

include(FindHOWL)
if (HOWL_FOUND)
	include_directories(${HOWL_INCLUDE_DIR})
	include_directories(${HOWL_INCLUDE_DIRS})
	link_directories(${HOWL_LIBRARY_DIR})
	link_libraries(${HOWL_LIBRARY})
	message(STATUS "HOWL: '${HOWL_LIBRARY}' -I${HOWL_INCLUDE_DIR} -I${HOWL_INCLUDE_DIRS}")
endif(HOWL_FOUND)

include(FindFlukso)
if (FLUKSO_FOUND)
	include_directories(${FLUKSO_INCLUDE_DIR})
	include_directories(${FLUKSO_INCLUDE_DIRS})
	link_directories(${FLUKSO_LIBRARY_DIR})
	link_libraries(${FLUKSO_LIBRARY})
	message(STATUS "LIBFLUKSO: '${FLUKSO_LIBRARY}' -I${FLUKSO_INCLUDE_DIR} -I${FLUKSO_INCLUDE_DIRS}")
endif(FLUKSO_FOUND)

# curl library
include(FindCurl)
if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIR})
  include_directories(${CURL_INCLUDE_DIRS})
  link_directories(${CURL_LIBRARY_DIR})
  link_libraries(${CURL_LIBRARY_SHARED})
  message(STATUS "CURL: '${CURL_LIBRARY_SHARED}' -I${CURL_INCLUDE_DIR} -I${CURL_INCLUDE_DIRS}")
endif(CURL_FOUND)

include(FindPopt)
if (POPT_FOUND)
  include_directories(${POPT_INCLUDE_DIR})
  include_directories(${POPT_INCLUDE_DIRS})
  link_directories(${POPT_LIBRARY_DIR})
  #IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  #  # there's a broken popt library in macports right now - only do
  #  # dynamic linking. this version seems to be ok.
  #  link_libraries(${POPT_LIBRARY_SHARED})
  #ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    link_libraries(${POPT_LIBRARY_SHARED})
	#ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message(STATUS "POPT: '${POPT_LIBRARY}' + '${POPT_LIBRARY_SHARED}' -I${POPT_INCLUDE_DIR} -I${POPT_INCLUDE_DIRS}")
else(POPT_FOUND)
  message(STATUS "POPT: No popt library found!")
endif(POPT_FOUND)

include(FindJSON)
if (JSON_FOUND)
  include_directories(${JSON_INCLUDE_DIR})
  include_directories(${JSON_INCLUDE_DIRS})
  link_directories(${JSON_LIBRARY_DIR})
  link_libraries(${JSON_LIBRARY})
  message(STATUS "JSON: '${JSON_LIBRARY}' -I${JSON_INCLUDE_DIR} -I${JSON_INCLUDE_DIRS}")
endif(JSON_FOUND)

# Reflect the package structure
add_subdirectory(src)

# add some files to the installation target
#INSTALL(FILES README LICENSE DESTINATION share/doc/libflukso)

# CPACK packaging
INCLUDE(InstallRequiredSystemLibraries)
	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Flukso Locator")
	SET(CPACK_PACKAGE_VENDOR "Stephan Platz")
	SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
	SET(CPACK_PACKAGE_VERSION_MAJOR ${V_MAJOR})
	SET(CPACK_PACKAGE_VERSION_MINOR ${V_MINOR})
	SET(CPACK_PACKAGE_VERSION_PATCH ${V_PATCH})
	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
	SET(CPACK_PACKAGE_EXECUTABLES "findFlukso" "Find a Flukso in the local network by Avahi")
	SET(CPACK_SOURCE_STRIP_FILES "")
	SET(CPACK_STRIP_FILES "bin/findFlukso")
#INCLUDE(CPack)


### print some output for the user
	message("")
	message("***** Configuration *****")
	message("  prefix: ${CMAKE_INSTALL_PREFIX}")

