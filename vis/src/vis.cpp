#include "vis/vis.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>

// We keep a record of the current width x height, its possible the 
// user resized the window and so we want to keep a record of the height
// and width to use later (setting projection matrices or viewports for exmaple)
static uint32_t window_width = 0;
static uint32_t window_height = 0;
GLFWwindow* window = nullptr;

void vis_init(const char* windowName,
              uint32_t width,
              uint32_t height) {
  #if defined(__APPLE__)
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
  if (!glfwInit()) {
    printf("GLFW ERROR: Failed to initialize\n");
    return;
  }

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  /* OSX Requires fordward compatibility */
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
  window_width = width;
  window_height = height;
  window = glfwCreateWindow(window_width, window_height, windowName, NULL, NULL);
  if (window == NULL) {
    printf("GLFW ERROR: Failed to create window\n");
    return;
  }
  // glfwSetFramebufferSizeCallback(window_, GLFW_ResizeCallback);
  // glfwSetDropCallback(window_, GLFW_DropCallback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("GLAD ERROR: Failed to initialize\n");
    return;
  }

  // Setup IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//  io.ConfigDockingWithShift = true;
//  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  return;
}

bool vis_is_window_running() {
  return !glfwWindowShouldClose(window);
}

void vis_frame_start() {
  glfwPollEvents();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void vis_frame_end() {
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
  glfwSwapBuffers(window);
}