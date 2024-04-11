cmake_minimum_required(VERSION 3.10)
project(cerberus_top C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

set(HIDAPI_FIND_COMPONENTS "libusb")
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")
find_package(HIDAPI REQUIRED)
include_directories(${HIDAPI_INCLUDE_DIRS})

add_subdirectory(zed)
add_executable(zed_main zed_main.c)
target_link_libraries(zed_main PUBLIC zed)

# add_subdirectory(external/zed-open-capture)