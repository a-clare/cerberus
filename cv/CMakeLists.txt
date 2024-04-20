project(cv)

include_directories(include)
add_library(${PROJECT_NAME}
            src/image.c)