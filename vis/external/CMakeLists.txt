add_subdirectory(glad)
add_subdirectory(glfw)

# Build imgui library, this essentially creates a single IMGUI library that can be linked against.
# Typically imgui files are "included" with your project but there may be multiple exe/libraries that
# want to use imgui so this makes it easier.
include_directories(imgui)
add_library(imgui
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        imgui/implot.cpp
        imgui/implot_demo.cpp
        imgui/implot_items.cpp)
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC imgui)
