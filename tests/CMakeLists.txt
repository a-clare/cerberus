cmake_minimum_required(VERSION 3.10)
project(cerberus_tests)

message(STATUS "CERBERUS_UNIT_TEST_SRCS is ${CERBERUS_UNIT_TEST_SRCS}")

find_package(GTest REQUIRED)
include_directories(${${GTEST_INCLUDE_DIR}})

add_executable(${PROJECT_NAME}
        main.cpp
        ${CERBERUS_UNIT_TEST_SRCS})

target_link_libraries(${PROJECT_NAME}
        GTest::GTest
        GTest::Main)

# This command will run the unit tests as part of the build
add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Running ${PROJECT_NAME}"
        POST_BUILD
        COMMAND ${PROJECT_NAME})