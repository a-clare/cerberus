cmake_minimum_required(VERSION 3.10)
project(cerberus C)

set(CMAKE_C_STANDARD 11)
add_library(cerberus 
  ../kitti_data_directory.c
  ../image.c)
target_include_directories(cerberus PUBLIC ${CMAKE_SOURCE_DIR})