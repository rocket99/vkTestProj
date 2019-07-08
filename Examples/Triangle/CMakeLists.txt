
file(GLOB src "*.cpp")

include_directories("${PROJECT_SOURCE_DIR}/src")
link_directories("$ENV{VULKAN_SDK}/lib")

add_executable(triangle ${src})

target_link_libraries(triangle PRIVATE TKMath TKBase TKNode)
