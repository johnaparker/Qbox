cmake_minimum_required(VERSION 3.5.2)
project(Apine)
set(CMAKE_BUILD_TYPE Release)
 
#set(CMAKE_CXX_COMPILER h5c++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

#Bring the headers, such as Student.h into the project
include_directories(include)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cc")
file(GLOB HEADERS "include/*.h")
 
#Generate the shared library from the sources
add_library(apine SHARED ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS apine DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/apine)

