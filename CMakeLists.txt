cmake_minimum_required(VERSION 3.10)
project(vkTestProj)

#SET(CMAKE_C_COMPILER "/usr/bin/clang")
#SET(CMAKE_C_FLAGS "-Wall -std=c99")
#SET(CMAKE_C_FLAGS_DEBUG "-g")
#SET(CMAKE_C_FLAGS_MINSIZEREL "-os -DNDEBUG")
#SET(CMAKE_C_FLAGS_RELEASE "-o4 -DNDEBUG")
#SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-o2 -g")

#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")
#SET(CMAKE_CXX_FLAGS_DEBUG "-g")
#SET(CMAKE_CXX_FLAGS_MINSIZEREL "-os -DNDEBUG")
#SET(CMAKE_CXX_FLAGS_RELEASE "-o4 -DNDEBUG")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-o2 -g")

message("start cmake")
message("${PROJECT_SOURCE_DIR}/include")
message("${PROJECT_SOURCE_DIR}/src")
message("complier : ${CMAKE_CXX_COMPILER_ID}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/Base")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/Math")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/Node")

add_subdirectory(src)
add_subdirectory(Examples)

