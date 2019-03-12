
#include_directories("${VULKAN_SDK}/include")

add_compile_options(-msse4 -mavx)

file(GLOB cpp_src "*.cpp")
file(GLOB header_src "*.h")
add_library(TKMath STATIC ${cpp_src} ${header_src})
target_link_libraries(TKMath vulkan jsoncpp jpeg png pthread xcb)

