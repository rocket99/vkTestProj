

message("${CMAKE_CURRENT_SRC_DIR}")

##include_directories("$ENV{VULKAN_SDK}/inlcude")
add_subdirectory("Base")
add_subdirectory("Math")
add_subdirectory("Node")

