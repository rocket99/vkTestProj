message("src dir:${CMAKE_CURRENT_SRC_DIR}")

#include_directories("${PROJECT_SOURCE_DIR}/src/Base")
#include_directories("${PROJECT_SOURCE_DIR}/src/Node")
#include_directories("${PROJECT_SOURCE_DIR}/src/Math")
#include_directories("$ENV{VULKAN_SDK}/include")

add_subdirectory("Math")
add_subdirectory("Base")
add_subdirectory("Node")

