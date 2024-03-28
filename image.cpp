#include "image.h"
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static constexpr uint32_t IMAGEU8_MAX_WIDTH  = 1600;
static constexpr uint32_t IMAGEU8_MAX_HEIGHT = 1200;
static constexpr uint32_t IMAGEU8_MAX_SIZE = IMAGEU8_MAX_WIDTH * IMAGEU8_MAX_HEIGHT;

/**
 * @brief Single 8bit channel image (grey scale for example)
 * 
 */
struct ImageU8 {
  uint32_t width = 0;
  uint32_t height = 0;
  std::array<uint8_t, IMAGEU8_MAX_SIZE> image;
};

ImageErrors image_read_u8(const char* path,
                          ImageU8Ptr* img) {

  *img = (ImageU8*)malloc(sizeof(ImageU8));

   // TODO: For now we are using stb_image to load png files. This library allocates
  // memory so we load the image and copy over to our expected data structure. Would be nice to 
  // load directly into our image and skip the copy
  int width = 0, height = 0, num_channels = 0;
  const unsigned char* raw_img = stbi_load(path, &width, &height, &num_channels, 1);
  if (raw_img == NULL) {
    return IMAGE_ERRORS_READ;
  }
  // Copy over the image from stbi's allocated memory into ours, and release stbi
  memcpy(&(*img)->image[0], raw_img, width * height);
  stbi_image_free((void*)raw_img);

  (*img)->height = height;
  (*img)->width  = width;
  return IMAGE_ERRORS_NONE;
}

ImageSize image_get_size_u8(const ImageU8Ptr img) {
  ImageSize ret;
  ret.height = img->height;
  ret.width  = img->width;
  return ret;
}

uint8_t* image_get_image_data_u8(const ImageU8Ptr img) {
  return &img->image[0];
}

void image_write(const ImageU8Ptr img) {
  stbi_write_png("/home/atc/cerberus/test.png", img->width, img->height, 1, &img->image[0], img->width);
}