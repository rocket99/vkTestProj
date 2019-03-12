

file(GLOB src "*.cpp")

add_executable(basic ${src})
target_link_libraries(basic PRIVATE TKMath TKBase TKNode)

