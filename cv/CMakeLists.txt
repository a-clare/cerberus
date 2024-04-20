project(cv)

include_directories(include)
add_library(${PROJECT_NAME}
  src/yuv_image.c)