cmake_minimum_required(VERSION 3.10)
project(cerberus_top C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# The unit tests need a direct file path (/some/path/to/a/file) for loading in test data
# or true data from a file
# We set a compile time definition/#define so the unit tests can find the data
add_compile_definitions(UNIT_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data/")

set(HIDAPI_FIND_COMPONENTS "libusb")
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")
find_package(HIDAPI REQUIRED)
include_directories(${HIDAPI_INCLUDE_DIRS})


enable_testing()
# Each subdirectory/library will add their own test.cpp files to this variable
# and then at the end we make one big unit test .exe in add_subdirectory(tests).
# This is done so we can avoid the code duplication of each library having to making its own
# exe, determining if they need to run the tests on build, and so on. Now we do it 
# once in tests/
set(CERBERUS_UNIT_TEST_SRCS "")

add_subdirectory(cv)
add_subdirectory(zed)
add_executable(zed_main zed_main.c)
target_link_libraries(zed_main PUBLIC zed)


if (CMAKE_TESTING_ENABLED)
  add_subdirectory(tests)
endif()
