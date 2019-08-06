
file (GLOB src_cpp "*.cpp")

link_directories("$ENV{VULKAN_SDK}/lib")

include_directories("${PRODUCT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/Math")
include_directories("${PROJECT_SOURCE_DIR}/src/Node")
include_directories("${PROJECT_SOURCE_DIR}/src/Math")

add_executable(poly ${src_cpp})

target_link_libraries(poly PRIVATE TKMath TKNode TKBase)
