add_executable(${PROJECT_NAME}_tests
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}_tests
        GTest::GTest
        GTest::Main)

# This command will run the unit tests as part of the build
add_custom_command(
        TARGET ${PROJECT_NAME}_tests
        COMMENT "Running ${PROJECT_NAME}_tests"
        POST_BUILD
        COMMAND ${PROJECT_NAME}_tests)