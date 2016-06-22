cmake_minimum_required(VERSION 3.5.2)
project(Qbox)
set(CMAKE_BUILD_TYPE Release)
enable_testing()
 
#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g -Og")
endif()

#Dependencies
find_library(h5_lib hdf5_cpp)
if(NOT h5_lib)
    message(FATAL_ERROR "hdf5 library not found")
else()
    message(STATUS "hdf5 library found")
endif()

#Include the headers
include_directories(include)
 
#Define the sources and headers
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB MAIN_HEADERS "include/*.h" )
file(GLOB MONITORS_HEADERS "include/monitors/*.h")
file(GLOB OBJECTS_HEADERS "include/objects/*.h")
file(GLOB SOURCES_HEADERS "include/sources/*.h")
 
#Generate the shared library from the sources
add_library(qbox SHARED ${SOURCES})
 
#Build executable
set (PROJECT_LINK_LIBS qbox ${h5_lib})
link_directories( build )
add_executable(emag tests/main.cpp)
target_link_libraries(emag ${PROJECT_LINK_LIBS} )

#Tests
#Simple Test
add_executable(simple tests/simple.cpp)
target_link_libraries(simple ${PROJECT_LINK_LIBS} )
add_test(SimpleRun simple)

#Set the location for library installation
install(TARGETS qbox DESTINATION lib)
install(FILES ${MAIN_HEADERS} DESTINATION include/qbox)
install(FILES ${MONITORS_HEADERS} DESTINATION include/qbox/monitors)
install(FILES ${OBJECTS_HEADERS} DESTINATION include/qbox/objects)
install(FILES ${SOURCES_HEADERS} DESTINATION include/qbox/sources)
install(FILES include/qbox.h DESTINATION include)
