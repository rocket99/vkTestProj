
#include_directories("${VULKAN_SDK}/include")
#include_directories("..")
include_directories("../Math")
include_directories("../Base")

file(GLOB cpp_src "*.cpp")
file(GLOB header_src "*.h")
add_library(TKNode STATIC ${cpp_src} ${header_src})
target_link_libraries(TKNode vulkan jsoncpp jpeg png pthread xcb)

