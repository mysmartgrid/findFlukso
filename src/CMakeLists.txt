# -*- mode: cmake; -*-
set(findFlukso_src findFlukso.cpp)

include_directories(${HOWL_INCLUDE_DIR}/howl ${CMAKE_SOURCE_DIR}/libflukso ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(findFlukso ${findFlukso_src})

# Link the executable 
target_link_libraries(findFlukso flukso pthread ${HOWL_LIBRARY} ${HOWL_LIBRARY_SHARED} ${POPT_LIBRARIES})

# add programs to the install target - do not install the hashbench benchmark program.
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/findFlukso
	  DESTINATION bin)

