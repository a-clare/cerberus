#ifndef CERBERUS_CV_IMAGE_H_
#define CERBERUS_CV_IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
  CV_IMAGE_TYPE_NONE,
  CV_IMAGE_TYPE_YUV,
} CV_IMAGE_TYPE;

typedef enum {
  CV_IMAGE_ERROR_NONE,
  CV_IMAGE_ERROR_READ,
  CV_IMAGE_ERROR_ALLOC,
  CV_IMAGE_ERROR_UNKNOWN_TYPE
} CV_IMAGE_ERROR;

typedef struct {
  uint32_t      width;
  uint32_t      height;
  uint32_t      channels;
  CV_IMAGE_TYPE type;
  uint8_t*      data;
} cv_Image;

/**
 * @brief Read an image from some path
 * 
 * Will allocate memory for img
 * 
 * @param path "/some/path/to/an/image/to/read"
 * @param img return image
 * @return CV_IMAGE_ERROR_NONE if successful,
 * @return CV_IMAGE_ERROR_READ if unable to open path, likely image does not exist or bad path
 * @return CV_IMAGE_ERROR_ALLOC if unable to allocate memory
 * @return CV_IMAGE_ERROR_UNKNOWN_TYPE the image at path is an unsupported type
 */
CV_IMAGE_ERROR cv_image_read(const char* path,
                             cv_Image** img);


#ifdef __cplusplus
}
#endif
#endif