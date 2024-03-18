#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageU8* image_u8_read(const char* path) {
  ImageU8* ret = (ImageU8*)malloc(sizeof(ImageU8));

   // TODO: For now we are using stb_image to load png files. This library allocates
  // memory so we load the image and copy over to our expected data structure. Would be nice to 
  // load directly into our image and skip the copy
  int width = 0, height = 0, num_channels = 0;
  const unsigned char* raw_img = stbi_load(path, &width, &height, &num_channels, 1);

  memcpy(&ret->image[0], raw_img, width * height);
  stbi_image_free((void*)raw_img);

  ret->height = height;
  ret->width  = width;
  return ret;
}