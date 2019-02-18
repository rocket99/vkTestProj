

message("${CMAKE_CURRENT_SRC_DIR}")
include_directories("${CMAKE_CURRENT_SRC_DIR}/../include")
include_directories("${CMAKE_CURRENT_SRC_DIR}/../include/Math")
include_directories("${CMAKE_CURRENT_SRC_DIR}/../include/Base")
include_directories("${CMAKE_CURRENT_SRC_DIR}/../include/Node")

add_compile_options(-msse4 -mavx)

file(GLOB BaseSrc "Base/*.cpp")
file(GLOB MathSrc "Math/*.cpp")
file(GLOB NodeSrc "Node/*.cpp")

add_library(TKEngine STATIC  ${BaseSrc} ${MathSrc} ${NodeSrc})
target_link_libraries(TKEngine vulkan jsoncpp jpeg png pthread xcb)

