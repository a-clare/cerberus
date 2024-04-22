project(cv)

include_directories(include)
include_directories(external)

add_library(${PROJECT_NAME}
            src/image.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)

if (CMAKE_TESTING_ENABLED)
  set(CERBERUS_UNIT_TEST_SRCS ${CERBERUS_UNIT_TEST_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cv_tests.cpp
    PARENT_SCOPE)
endif()