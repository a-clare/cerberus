#include "cv/image.h"
#include <string.h>

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