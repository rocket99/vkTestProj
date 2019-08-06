
message("configure:${CMAKE_CURRENT_SOURCE_DIR}")
message("configure:${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB cpp_src "*.cpp")
file(GLOB header_src "*.h")

include_directories("$ENV{VULKAN_SDK}/include")

link_directories("$ENV{VULKAN_SDK}/lib")

add_library(TKBase STATIC ${cpp_src} ${header_src})

target_link_libraries(TKBase vulkan jsoncpp jpeg png pthread xcb)
target_include_directories(TKBase PUBLIC "$ENV{VULKAN_SDK}/include")

