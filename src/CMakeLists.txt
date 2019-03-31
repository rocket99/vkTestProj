

message("${CMAKE_CURRENT_SRC_DIR}")

include_directories("$ENV{VULKAN_SDK}/inlcude")

#include_directories("${CMAKE_CURRENT_SRC_DIR}/.")
#include_directories("${CMAKE_CURRENT_SRC_DIR}/Math")
#include_directories("${CMAKE_CURRENT_SRC_DIR}/Base")
#include_directories("${CMAKE_CURRENT_SRC_DIR}/Node")

add_subdirectory("Base")
add_subdirectory("Math")
add_subdirectory("Node")

