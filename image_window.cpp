#include "image_window.h"
#include <glad/glad.h>
#include <cstdlib>

struct ImageWindow {
  GLuint tex = 0;
  ImageU8* img = nullptr;
};

ImageWindowPtr image_window_create(ImageU8Ptr img) {
  ImageWindowPtr ret = (ImageWindowPtr)malloc(sizeof(ImageWindow));
  if (ret == nullptr) {
    return nullptr;
  }
  ret->img = img;
  glGenTextures(1, &ret->tex);
  glBindTexture(GL_TEXTURE_2D, ret->tex);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
  const ImageSize img_size = image_get_size_u8(img);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, img_size.width, img_size.height, 0, GL_RED, GL_UNSIGNED_BYTE, image_get_image_data_u8(img));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
  glBindTexture(GL_TEXTURE_2D, 0);

  return ret;
}

void image_window_draw(const ImageWindowPtr win,
                       const char* windowName) {
  if (ImGui::Begin(windowName)) {
    const auto s = image_get_size_u8(win->img);
    ImGui::Image((void*)(intptr_t)win->tex, ImVec2(s.width, s.height));
  }
  ImGui::End();
}