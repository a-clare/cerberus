#include "cv/image.h"
#include <string.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef enum {
  CV_FILE_EXT_UNKNOWN,
  CV_FILE_EXT_PNG,
  CV_FILE_EXT_BIN
} CV_FILE_EXT;

static CV_FILE_EXT cv_get_file_ext(const char* path);

CV_IMAGE_ERROR cv_image_read(const char* path,
                             cv_Image** img) {
  CV_IMAGE_ERROR ret = CV_IMAGE_ERROR_NONE;

  CV_FILE_EXT ext = cv_get_file_ext(path);
  if (ext == CV_FILE_EXT_UNKNOWN) {
    return CV_IMAGE_ERROR_UNKNOWN_TYPE;
  }

  switch(ext) {
    case CV_FILE_EXT_PNG:
      *img = (cv_Image*)malloc(sizeof(cv_Image));

       // TODO: For now we are using stb_image to load png files. This library allocates
      // memory so we load the image and copy over to our expected data structure. Would be nice to 
      // load directly into our image and skip the copy
      int width = 0, height = 0, num_channels = 0;
      const unsigned char* raw_img = stbi_load(path, &width, &height, &num_channels, 1);
      if (raw_img == NULL) {
        return CV_IMAGE_ERROR_ALLOC;
      }
      (*img)->data = malloc(width * height * num_channels);
      if ((*img)->data == NULL) {
        free(*img);
        return CV_IMAGE_ERROR_ALLOC;
      }
      // Copy over the image from stbi's allocated memory into ours, and release stbi
      memcpy(&(*img)->data[0], raw_img, width * height * num_channels);
      stbi_image_free((void*)raw_img);

      (*img)->height   = height;
      (*img)->width    = width;
      (*img)->channels = num_channels;
      break;
    case CV_FILE_EXT_BIN:
      break;  
  }
  return CV_IMAGE_ERROR_NONE;
}

static CV_FILE_EXT cv_get_file_ext(const char* path) {
  // We are assuming the extension is going to be after the last '.' found in the string.
  const char* extension = strrchr(path, '.');
  // No extension found
  if (extension == NULL) {
    return CV_FILE_EXT_UNKNOWN;
  }

  // Move past the '.'
  extension++;

  if (strcmp(extension, "bin") == 0) {
    return CV_FILE_EXT_BIN;
  }
  else if (strcmp(extension, "png") == 0) {
    return CV_FILE_EXT_PNG;
  }

  return CV_FILE_EXT_UNKNOWN;
}