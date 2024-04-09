cmake_minimum_required(VERSION 3.10)
project(cerberus_top)

add_subdirectory(zed)
add_executable(zed_main zed_main.c)
target_link_libraries(zed_main PUBLIC zed)

# add_subdirectory(external/zed-open-capture)