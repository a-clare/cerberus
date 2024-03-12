cmake_minimum_required(VERSION 3.10)
project(cerberus)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp kitti_data_directory.cpp)

add_subdirectory(tests)