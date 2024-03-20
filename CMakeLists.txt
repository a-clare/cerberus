cmake_minimum_required(VERSION 3.10)
project(cerberus)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(vis/external)
add_subdirectory(vis)

add_library(clib 
  kitti_data_directory.cpp 
  image_window.cpp
  image.cpp)
target_include_directories(clib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(clib PUBLIC vis)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC vis clib)
add_subdirectory(tests)