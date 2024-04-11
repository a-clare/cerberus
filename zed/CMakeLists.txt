cmake_minimum_required(VERSION 3.10)

project(zed)

include_directories()

add_library(${PROJECT_NAME}
  src/sensor_capture.c
  src/errors.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE 
  m 
  ${HIDAPI_LIBRARIES})