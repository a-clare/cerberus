cmake_minimum_required(VERSION 3.10)

project(zed)

include_directories(include)

add_library(${PROJECT_NAME}
  src/sensor_capture.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)  