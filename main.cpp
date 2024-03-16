#include <dirent.h>
#include <string.h>
#include <glad/glad.h>

#include <cstdio>
#include "kitti_data_directory.h"
#include "logger.h"
#include "vis/vis.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    LOG("%s", "No input directory provided");
    return 0;
  }

  vis_init("Cerberus", 1200, 1080);
  const char *input_dir = argv[1];
  LOG("Input directory is %s", input_dir);

  KittiDataDirPtr ptr;
  KittiDataDirErrors err = kitti_data_dir_create(input_dir, &ptr);
  if (err != KITTI_DATA_DIR_ERRORS_NONE) {
    LOG("Kitti data directory error %d", err);
    return 0;
  }
  StereoImage stereo_image;
  kitti_data_dir_load_img(ptr, 0, 0, &stereo_image);

  // Create a OpenGL texture identifier
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stereo_image.width, stereo_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stereo_image.left);
  while (vis_is_window_running()) {
    vis_frame_start();

    ImGui::Begin("Image");

    ImGui::Image((void*)(intptr_t)image_texture, ImVec2(stereo_image.width * 0.5f, stereo_image.height* 0.5f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
    ImGui::End();
    vis_frame_end();
  }
  return 0;
}