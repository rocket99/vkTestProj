#
file(GLOB src "*.cpp")
add_executable(light ${src})
target_link_libraries(light PRIVATE TKEngine)

