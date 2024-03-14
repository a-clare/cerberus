project(vis)

include_directories(include)
add_library(${PROJECT_NAME} src/vis.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui glad glfw)
target_include_directories(${PROJECT_NAME} PUBLIC include)