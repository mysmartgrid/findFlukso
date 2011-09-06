# -*- mode: cmake; -*-
set(findFlukso_src findFlukso.cpp)

include_directories(${HOWL_INCLUDE_DIR}/howl ${FLUKSO_INCLUDE_DIR}/flukso ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(findFlukso ${findFlukso_src})

# Link the executable 
target_link_libraries(findFlukso pthread ${HOWL_LIBRARY} ${HOWL_LIBRARY_SHARED} ${FLUKSO_LIBRARY} ${POPT_LIBRARIES})

# add programs to the install target - do not install the hashbench benchmark program.
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/findFlukso
	  DESTINATION bin)

