cmake_minimum_required(VERSION 3.10)
project(cerberus_top)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(cerberus)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC cerberus m)
add_subdirectory(tests)