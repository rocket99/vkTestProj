

file(GLOB src "*.cpp")
link_directories("$ENV{VULKAN_SDK}/lib")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/Base")
include_directories("${PROJECT_SOURCE_DIR}/src/Node")
include_directories("${PROJECT_SOURCE_DIR}/src/Math")
add_executable(basic ${src})
target_link_libraries(basic PRIVATE TKMath TKBase TKNode)

