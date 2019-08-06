cmake_minimum_required(VERSION 3.10)
project(vkTestProj)

#SET(CMAKE_C_COMPILER "/usr/bin/clang")
#SET(CMAKE_C_FLAGS "-Wall -std=c99")
#SET(CMAKE_C_FLAGS_DEBUG "-g")
#SET(CMAKE_C_FLAGS_MINSIZEREL "-os -DNDEBUG")
#SET(CMAKE_C_FLAGS_RELEASE "-o4 -DNDEBUG")
#SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-o2 -g")

SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")
#SET(CMAKE_CXX_FLAGS_DEBUG "-g")
#SET(CMAKE_CXX_FLAGS_MINSIZEREL "-os -DNDEBUG")
#SET(CMAKE_CXX_FLAGS_RELEASE "-o4 -DNDEBUG")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-o2 -g")

message("start cmake")
message("${PROJECT_SOURCE_DIR}")
message("c++ complier : ${CMAKE_CXX_COMPILER_ID}")
message("$ENV{VULKAN_SDK}/include")

#include_directories("$ENV{VULKAN_SDK}/include")
#include_directories("${PROJECT_SOURCE_DIR}/src")
#include_directories("${PROJECT_SOURCE_DIR}/src/Base")
#include_directories("${PROJECT_SOURCE_DIR}/src/Math")
#include_directories("${PROJECT_SOURCE_DIR}/src/Node")

find_library(VulkanLib NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib")
message("find vulkan library: ${VulkanLib}")
#link_libraries(${VulkanLib})

add_subdirectory(src)
add_subdirectory(Examples)

