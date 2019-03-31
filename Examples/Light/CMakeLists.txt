
file(GLOB src "*.cpp")
link_directories("$ENV{VULKAN_SDK}/lib")
add_executable(light ${src})
target_link_libraries(light PRIVATE TKBase TKMath TKNode)

