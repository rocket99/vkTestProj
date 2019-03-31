

file(GLOB src "*.cpp")
link_directories("$ENV{VULKAN_SDK}/lib")
add_executable(basic ${src})
target_link_libraries(basic PRIVATE TKMath TKBase TKNode)

