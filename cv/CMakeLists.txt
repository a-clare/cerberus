project(cv)

include_directories(include)
add_library(${PROJECT_NAME}
            src/image.c)

if (CMAKE_TESTING_ENABLED)
  set(CERBERUS_UNIT_TEST_SRCS ${CERBERUS_UNIT_TEST_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cv_tests.cpp
    PARENT_SCOPE)
endif()