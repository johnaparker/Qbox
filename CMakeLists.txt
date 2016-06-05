cmake_minimum_required(VERSION 3.5.2)
project(Qbox)
set(CMAKE_BUILD_TYPE Release)
 
#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

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
set (PROJECT_LINK_LIBS libqbox.so hdf5_cpp)
link_directories( build )
add_executable(emag main.cc)
target_link_libraries(emag ${PROJECT_LINK_LIBS} )

#Set the location for library installation
install(TARGETS qbox DESTINATION lib)
install(FILES ${MAIN_HEADERS} DESTINATION include/qbox)
install(FILES ${MONITORS_HEADERS} DESTINATION include/qbox/monitors)
install(FILES ${OBJECTS_HEADERS} DESTINATION include/qbox/objects)
install(FILES ${SOURCES_HEADERS} DESTINATION include/qbox/sources)
install(FILES include/qbox.h DESTINATION include)

